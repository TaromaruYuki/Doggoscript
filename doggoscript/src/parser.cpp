#include <doggoscript/parser.hpp>

ParseResult Parser::parse() {
    ParseResult result = this->statements();

    if (!result.should_return() && this->current_token.has_value() &&
        this->current_token.value().type != TokenType::FileEnd) {
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '+', '-', '*', or '/'"
        ));
    }

    return result;
}

void Parser::advance() {
    this->index++;
    this->update_current_token();
}

void Parser::reverse(int amount) {
    this->index -= amount;
    this->update_current_token();
}

void Parser::update_current_token() {
    if (this->index >= 0 && this->index < this->tokens.size())
        this->current_token = this->tokens[this->index];
    else
        this->current_token = std::nullopt;
}

ParseResult Parser::statements() {
    ParseResult result;
    std::vector<BaseNode *> statements;
    Position start_pos = Position(this->current_token.value().start_pos.value());

    while (this->current_token.has_value() && this->current_token.value().type == TokenType::NewLine) {
        result.register_advancement();
        this->advance();
    }

    BaseNode *statement = result.reg(this->statement());
    if (result.should_return()) return result;

    statements.push_back(statement);
    bool more_statements = true;

    while (true) {
        int newline_count = 0;

        while (this->current_token.has_value() && this->current_token.value().type == TokenType::NewLine) {
            result.register_advancement();
            this->advance();
            newline_count++;
        }

        if (newline_count == 0)
            more_statements = false;

        if (!more_statements)
            break;

        std::optional<BaseNode *> statement = result.try_reg(this->statement());
        if (!statement.has_value()) {
            this->reverse(result.to_reverse_count);
            more_statements = false;
            continue;
        }

        statements.push_back(statement.value());
    }

    return *result.success(new StatementsNode(statements, start_pos, this->current_token.value().end_pos.value()));
}

ParseResult Parser::statement() {
    ParseResult result;
    Token current_token = this->current_token.value();
    Position start_pos = Position(current_token.start_pos.value());

    if (current_token.is_keyword("return")) {
        result.register_advancement();
        this->advance();

        std::optional<BaseNode *> expr = result.try_reg(this->expr());
        BaseNode *return_node;

        if (!expr.has_value()) {
            this->reverse(result.to_reverse_count);
            return_node = nullptr;
        } else
            return_node = expr.value();

        return *result.success(new ReturnNode(return_node, start_pos, this->current_token.value().end_pos.value()));
    } else if (current_token.is_keyword("continue")) {
        result.register_advancement();
        this->advance();

        return *result.success(new ContinueNode(start_pos, this->current_token.value().end_pos.value()));
    } else if (current_token.is_keyword("break")) {
        result.register_advancement();
        this->advance();

        return *result.success(new BreakNode(start_pos, this->current_token.value().end_pos.value()));
    } else if(current_token.is_keyword("incl")) {
        result.register_advancement();
        this->advance();

        if(this->current_token.value().type != TokenType::String) {
            return *result.failure(InvalidSyntaxError(
                current_token.start_pos.value(), current_token.end_pos.value(),
                "Expected string"
            ));
        }

        Token path = this->current_token.value();
        result.register_advancement();
        this->advance();

        return *result.success(new IncludeNode(path, start_pos, this->current_token.value().end_pos.value()));
    }

    BaseNode *expr = result.reg(this->expr());
    if (result.should_return())
        return *result.failure(InvalidSyntaxError(
                current_token.start_pos.value(), current_token.end_pos.value(),
                "Expected 'return', 'continue', 'break', 'val', 'Int', 'Float', 'Variable', '[', '+', '-', '!', or '('"
        ));

    return *result.success(expr);
}

ParseResult Parser::expr() {
    ParseResult result;
    Token current_token = this->current_token.value();

    if (current_token.is_keyword("val")) {
        result.register_advancement();
        this->advance();

        if (this->current_token.value().type != TokenType::Variable) {
            return *result.failure(InvalidSyntaxError(
                    current_token.start_pos.value(), current_token.end_pos.value(),
                    "Expected variable name"
            ));
        }

        Token variable_token = this->current_token.value();
        result.register_advancement();
        this->advance();

        std::vector<Token> children;

        while(this->current_token.value().type == TokenType::Arrow) {
            result.register_advancement();
            this->advance();

            if (this->current_token.value().type != TokenType::Variable) {
                return *result.failure(InvalidSyntaxError(
                        current_token.start_pos.value(), current_token.end_pos.value(),
                        "Expected variable name"
                ));
            }

            children.push_back(this->current_token.value());
            result.register_advancement();
            this->advance();
        }

        if (!this->current_token.value().is_keyword("is")) {
            return *result.failure(InvalidSyntaxError(
                    current_token.start_pos.value(), current_token.end_pos.value(),
                    "Expected keyword 'is'"
            ));
        }

        result.register_advancement();
        this->advance();

        BaseNode *value = result.reg(this->expr());
        if (result.should_return()) return result;

        return *result.success(new VariableAssignmentNode(variable_token, children, value));
    }

    BaseNode *node = result.reg(
            this->bin_op([this] { return this->comp_expr(); }, {TokenType::LogicalAnd, TokenType::LogicalOr}));

    if (result.should_return())
        return *result.failure(InvalidSyntaxError(
                current_token.start_pos.value(), current_token.end_pos.value(),
                "Expected 'val', 'Int', 'Float', 'Variable', '[', '+', '-', '!', or '('"
        ));

    return *result.success(node);
}

ParseResult Parser::comp_expr() {
    ParseResult result;
    Token current_token = this->current_token.value();

    if (current_token.type == TokenType::LogicalNot) {
        result.register_advancement();
        this->advance();

        BaseNode *node = result.reg(this->comp_expr());
        if (result.should_return()) return result;

        return *result.success(new UnaryOperationNode(current_token, node));
    }

    BaseNode *node = result.reg(this->bin_op([this] { return this->arith_expr(); },
                                             {TokenType::EqualsTo, TokenType::NotEquals, TokenType::LessThan,
                                              TokenType::GreaterThan, TokenType::LessThanEquals,
                                              TokenType::GreaterThanEquals}));

    if (result.should_return())
        return *result.failure(InvalidSyntaxError(
                current_token.start_pos.value(), current_token.end_pos.value(),
                "Expected 'val', 'Int', 'Float', 'Variable', '[', '+', '-', '!', or '('"
        ));

    return *result.success(node);
}

ParseResult Parser::arith_expr() {
    std::vector<TokenType> ops({TokenType::Plus, TokenType::Minus});
    std::function < ParserFunction > func = [this] { return this->term(); };
    return this->bin_op(func, ops);
}

ParseResult Parser::term() {
    std::vector<TokenType> ops({TokenType::Multiply, TokenType::Divide});
    std::function < ParserFunction > func = [this] { return this->factor(); };
    return this->bin_op(func, ops);
}

ParseResult Parser::factor() {
    ParseResult result;
    Token current_token = this->current_token.value();

    if (current_token.type == TokenType::Plus || current_token.type == TokenType::Minus) {
        result.register_advancement();
        this->advance();

        BaseNode *node = result.reg(this->factor());
        if (result.should_return()) return result;

        return *result.success(new UnaryOperationNode(current_token, node));
    }

    BaseNode *node = result.reg(this->power());
    if (result.should_return()) return result;

    return *result.success(node);
}

ParseResult Parser::power() {
    std::vector<TokenType> ops({TokenType::Power});
    std::function < ParserFunction > func = [this] { return this->call(); };
    std::function < ParserFunction > func2 = [this] { return this->factor(); };
    return this->bin_op(func, ops, func2);
}

ParseResult Parser::call() {
    ParseResult result;
    BaseNode *atom = result.reg(this->atom());
    if (result.should_return()) return result;

    if (this->current_token.value().type == TokenType::LParen) {
        result.register_advancement();
        this->advance();

        std::vector<BaseNode *> arg_nodes;

        if (this->current_token.value().type == TokenType::RParen) {
            result.register_advancement();
            this->advance();
        } else {
            arg_nodes.push_back(result.reg(this->expr()));
            if (result.should_return()) return result;

            while (this->current_token.value().type == TokenType::Comma) {
                result.register_advancement();
                this->advance();

                arg_nodes.push_back(result.reg(this->expr()));
                if (result.should_return()) return result;
            }

            if (this->current_token.value().type != TokenType::RParen) {
                return *result.failure(InvalidSyntaxError(
                        this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                        "Expected ',' or ')'"
                ));
            }

            result.register_advancement();
            this->advance();
        }

        return *result.success(new CallNode(atom, arg_nodes));
    }

    return *result.success(atom);
}

ParseResult Parser::atom() {
    ParseResult result;
    Token value = this->current_token.value();

    if (value.type == TokenType::Int || value.type == TokenType::Float) {
        result.register_advancement();
        this->advance();

        return *result.success(new NumberNode(value));
    } else if (value.type == TokenType::String) {
        result.register_advancement();
        this->advance();

        return *result.success(new StringNode(value));
    } else if (value.type == TokenType::Identifier) {
        result.register_advancement();
        this->advance();

        return *result.success(new IdentifierNode(value));
    } else if (value.type == TokenType::Variable) {
        result.register_advancement();
        this->advance();

        std::vector<Token> children;

        while(this->current_token.has_value() && this->current_token.value().type == TokenType::Arrow) {
            result.register_advancement();
            this->advance();

            if (this->current_token.value().type != TokenType::Variable && this->current_token.value().type != TokenType::Identifier) {
                return *result.failure(InvalidSyntaxError(
                        value.start_pos.value(), value.end_pos.value(),
                        "Expected variable name or function name"
                ));
            }

            children.push_back(this->current_token.value());
            result.register_advancement();
            this->advance();
        }

        if (this->current_token.has_value() && this->current_token.value().is_keyword("is")) {
            result.register_advancement();
            this->advance();

            BaseNode *expr = result.reg(this->expr());
            if (result.should_return()) return result;

            return *result.success(new VariableReassignmentNode(value, children, expr));
        }

        return *result.success(new VariableAccessNode(value, children));
    } else if (value.is_keyword("true") || value.is_keyword("false")) {
        result.register_advancement();
        this->advance();

        return *result.success(new BooleanNode(value));
    } else if (value.type == TokenType::LParen) {
        result.register_advancement();
        this->advance();

        BaseNode *expr = result.reg(this->expr());
        if (result.should_return()) return result;

        if (this->current_token.value().type != TokenType::RParen) {
            return *result.failure(InvalidSyntaxError(
                    this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                    "Expected ')'"
            ));
        }

        return *result.success(expr);
    } else if (value.type == TokenType::LSquare) {
        BaseNode *list_expr = result.reg(this->list_expr());
        if (result.should_return()) return result;

        return *result.success(list_expr);
    } else if (value.type == TokenType::LCurly) {
        BaseNode *dict_expr = result.reg(this->dict_expr());
        if (result.should_return()) return result;

        return *result.success(dict_expr);
    } else if (value.is_keyword("if")) {
        BaseNode *if_expr = result.reg(this->if_expr());
        if (result.should_return()) return result;

        return *result.success(if_expr);
    } else if (value.is_keyword("for")) {
        BaseNode *for_expr = result.reg(this->for_expr());
        if (result.should_return()) return result;

        return *result.success(for_expr);
    } else if (value.is_keyword("while")) {
        BaseNode *while_expr = result.reg(this->while_expr());
        if (result.should_return()) return result;

        return *result.success(while_expr);
    } else if (value.is_keyword("fn")) {
        BaseNode *fun_expr = result.reg(this->func_def());
        if (result.should_return()) return result;

        return *result.success(fun_expr);
    } else if(value.is_keyword("class")) {
        BaseNode *class_expr = result.reg(this->class_def());
        if (result.should_return()) return result;

        return *result.success(class_expr);
    }

    return *result.failure(InvalidSyntaxError(
            value.start_pos.value(), value.end_pos.value(),
            "Expected 'val', 'Int', 'Float', 'Variable', '[', '+', '-', '!', or '('"
    ));
}

ParseResult Parser::list_expr() {
    ParseResult result;
    Position start_pos = Position(this->current_token.value().start_pos.value());
    std::vector<BaseNode *> elements;

    if (this->current_token.value().type != TokenType::LSquare) {
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '['"
        ));
    }

    result.register_advancement();
    this->advance();

    if (this->current_token.value().type == TokenType::RSquare) {
        result.register_advancement();
        this->advance();
    } else {
        elements.push_back(result.reg(this->expr()));
        if (result.should_return())
            return *result.failure(InvalidSyntaxError(
                    this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                    "Expected ']'"
            ));

        while (this->current_token.value().type == TokenType::Comma) {
            result.register_advancement();
            this->advance();

            elements.push_back(result.reg(this->expr()));
            if (result.should_return()) return result;
        }

        if (this->current_token.value().type != TokenType::RSquare) {
            return *result.failure(InvalidSyntaxError(
                    this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                    "Expected ',' or ']'"
            ));
        }

        result.register_advancement();
        this->advance();
    }

    return *result.success(new ListNode(elements, start_pos, this->current_token.value().end_pos.value()));
}

ParseResult Parser::if_expr() {
    ParseResult result;
    std::vector<std::tuple<BaseNode *, BaseNode *, bool>> cases;
    BaseNode *else_case = nullptr;

    if (!this->current_token.value().is_keyword("if")) {
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected 'if'"
        ));
    }

    result.register_advancement();
    this->advance();

    BaseNode *condition = result.reg(this->expr());
    if (result.should_return()) return result;

    if (this->current_token.value().type != TokenType::LCurly)
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '{'"
        ));

    result.register_advancement();
    this->advance();

    BaseNode *expr = result.reg(this->statements());
    if (result.should_return()) return result;

    cases.emplace_back(condition, expr, static_cast<StatementsNode *>(expr)->statements.size() <= 1);

    if (this->current_token.value().type != TokenType::RCurly)
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '}'"
        ));

    result.register_advancement();
    this->advance();

    while (this->current_token.value().is_keyword("else")) {
        result.register_advancement();
        this->advance();

        if (this->current_token.value().is_keyword("if")) {
            result.register_advancement();
            this->advance();

            BaseNode *condition = result.reg(this->expr());
            if (result.should_return()) return result;

            if (this->current_token.value().type != TokenType::LCurly)
                return *result.failure(InvalidSyntaxError(
                        this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                        "Expected '{'"
                ));

            result.register_advancement();
            this->advance();

            BaseNode *expr = result.reg(this->statements());
            if (result.should_return()) return result;

            cases.emplace_back(condition, expr, static_cast<StatementsNode *>(expr)->statements.size() <= 1);

            if (this->current_token.value().type != TokenType::RCurly)
                return *result.failure(InvalidSyntaxError(
                        this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                        "Expected '}'"
                ));

            result.register_advancement();
            this->advance();
        } else {
            if (this->current_token.value().type != TokenType::LCurly)
                return *result.failure(InvalidSyntaxError(
                        this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                        "Expected '{'"
                ));

            result.register_advancement();
            this->advance();

            else_case = result.reg(this->statements());
            if (result.should_return()) return result;

            if (this->current_token.value().type != TokenType::RCurly)
                return *result.failure(InvalidSyntaxError(
                        this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                        "Expected '}'"
                ));

            result.register_advancement();
            this->advance();
            break;
        }
    }

    bool else_case_should_return = false;
    std::optional<std::tuple<BaseNode *, bool>> return_else_case;
    if (else_case != nullptr) {
        else_case_should_return = static_cast<StatementsNode *>(else_case)->statements.size() <= 1;
        return_else_case = std::make_tuple(else_case, else_case_should_return);
    }

    return *result.success(new IfNode(cases, return_else_case));
}

ParseResult Parser::for_expr() {
    ParseResult result;

    if (!this->current_token.value().is_keyword("for")) {
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected 'for'"
        ));
    }

    result.register_advancement();
    this->advance();

    if (this->current_token.value().type != TokenType::Variable) {
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected variable name"
        ));
    }

    Token variable_token = this->current_token.value();
    result.register_advancement();
    this->advance();

    if (!this->current_token.value().is_keyword("in")) {
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected keyword 'in'"
        ));
    }

    result.register_advancement();
    this->advance();

    BaseNode *start_value = result.reg(this->expr());
    if (result.should_return()) return result;

    if (this->current_token.value().type != TokenType::Through)
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '..'"
        ));

    result.register_advancement();
    this->advance();

    BaseNode *end_value = result.reg(this->expr());
    if (result.should_return()) return result;

    BaseNode *step_value = nullptr;
    if (this->current_token.value().type == TokenType::Through) {
        result.register_advancement();
        this->advance();

        step_value = result.reg(this->expr());
        if (result.should_return()) return result;
    }

    if (this->current_token.value().type != TokenType::LCurly)
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '{'"
        ));

    result.register_advancement();
    this->advance();

    BaseNode *body = result.reg(this->statements());
    if (result.should_return()) return result;

    if (this->current_token.value().type != TokenType::RCurly)
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '}'"
        ));

    result.register_advancement();
    this->advance();

    return *result.success(new ForNode(variable_token, start_value, end_value, step_value, body,
                                       static_cast<StatementsNode *>(body)->statements.size() <= 1));
}

ParseResult Parser::while_expr() {
    ParseResult result;

    if (!this->current_token.value().is_keyword("while")) {
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected 'while'"
        ));
    }

    result.register_advancement();
    this->advance();

    BaseNode *condition = result.reg(this->expr());
    if (result.should_return()) return result;

    if (this->current_token.value().type != TokenType::LCurly)
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '{'"
        ));

    result.register_advancement();
    this->advance();

    BaseNode *body = result.reg(this->statements());
    if (result.should_return()) return result;

    if (this->current_token.value().type != TokenType::RCurly)
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '}'"
        ));

    result.register_advancement();
    this->advance();

    return *result.success(new WhileNode(condition, body, static_cast<StatementsNode *>(body)->statements.size() <= 1));
}

ParseResult Parser::func_def() {
    ParseResult result;

    if (!this->current_token.value().is_keyword("fn")) {
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected 'fn'"
        ));
    }

    result.register_advancement();
    this->advance();

    std::optional<Token> name_token = std::nullopt;

    if (this->current_token.value().type == TokenType::Identifier) {
        name_token = this->current_token.value();
        result.register_advancement();
        this->advance();
    }

    if (this->current_token.value().type != TokenType::LParen)
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '('"
        ));

    result.register_advancement();
    this->advance();

    std::vector<Token *> arg_name_tokens;

    if (this->current_token.value().type == TokenType::Variable) {
        arg_name_tokens.push_back(new Token(this->current_token.value()));
        result.register_advancement();
        this->advance();

        while (this->current_token.value().type == TokenType::Comma) {
            result.register_advancement();
            this->advance();

            if (this->current_token.value().type != TokenType::Variable)
                return *result.failure(InvalidSyntaxError(
                        this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                        "Expected variable name"
                ));

            arg_name_tokens.push_back(new Token(this->current_token.value()));
            result.register_advancement();
            this->advance();
        }

        if (this->current_token.value().type != TokenType::RParen)
            return *result.failure(InvalidSyntaxError(
                    this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                    "Expected ')' or variable name"
            ));
    } else {
        if (this->current_token.value().type != TokenType::RParen)
            return *result.failure(InvalidSyntaxError(
                    this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                    "Expected ')' or variable name"
            ));
    }

    result.register_advancement();
    this->advance();

    if (this->current_token.value().type != TokenType::LCurly)
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '{'"
        ));

    result.register_advancement();
    this->advance();

    BaseNode *body = result.reg(this->statements());
    if (result.should_return()) return result;

    if (this->current_token.value().type != TokenType::RCurly)
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '}'"
        ));

    result.register_advancement();
    this->advance();

    return *result.success(new FunctionDefinitionNode(name_token, arg_name_tokens, body,
                                                      static_cast<StatementsNode *>(body)->statements.size() <= 1, true));
}

ParseResult Parser::bin_op(const std::function<ParserFunction> &func, std::vector<TokenType> ops,
                           std::optional<std::function<ParserFunction>> func2) {
    ParseResult result;
    if (!func2.has_value())
        func2 = func;
    BaseNode *left = result.reg(func());
    if (result.should_return()) return result;

    while (
            this->current_token.has_value() &&
            std::find(ops.begin(), ops.end(), this->current_token.value().type) != ops.end()
            ) {
        Token op_tok = this->current_token.value();
        this->advance();

        BaseNode *right = result.reg(func2.value()());
        if (result.should_return()) return result;
        auto *node = new BinaryOperationNode(left, op_tok, right);

        left = node;
    }

    return *result.success(left);
}

ParseResult Parser::dict_expr() {
    ParseResult result;
    Position start_pos = Position(this->current_token.value().start_pos.value());

    if (this->current_token.value().type != TokenType::LCurly) {
        return *result.failure(InvalidSyntaxError(
                this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                "Expected '{'"
        ));
    }

    result.register_advancement();
    this->advance();

    std::vector<std::tuple<BaseNode *, BaseNode *>> elements;

    if (this->current_token.value().type == TokenType::RCurly) {
        result.register_advancement();
        this->advance();
    } else {
        BaseNode *key = result.reg(this->atom());
        if (result.should_return()) return result;

        if (this->current_token.value().type != TokenType::Arrow) {
            return *result.failure(InvalidSyntaxError(
                    this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                    "Expected '->'"
            ));
        }

        result.register_advancement();
        this->advance();

        BaseNode *value = result.reg(this->expr());
        if (result.should_return()) return result;

        elements.emplace_back(key, value);

        while (this->current_token.value().type == TokenType::Comma) {
            result.register_advancement();
            this->advance();

            key = result.reg(this->atom());
            if (result.should_return()) return result;

            if (this->current_token.value().type != TokenType::Arrow) {
                return *result.failure(InvalidSyntaxError(
                        this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                        "Expected '->'"
                ));
            }

            result.register_advancement();
            this->advance();

            value = result.reg(this->expr());
            if (result.should_return()) return result;

            elements.emplace_back(key, value);
        }

        if (this->current_token.value().type != TokenType::RCurly) {
            return *result.failure(InvalidSyntaxError(
                    this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
                    "Expected '}'"
            ));
        }

        result.register_advancement();
        this->advance();
    }

    return *result.success(new DictNode(elements, start_pos, this->current_token.value().end_pos.value()));
}

ParseResult Parser::class_def() {
    ParseResult result;
    Position start_pos(this->current_token.value().start_pos.value());

    if(!this->current_token.value().is_keyword("class")) {
        return *result.failure(InvalidSyntaxError(
            this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
            "Expected 'class'"
        ));
    }

    result.register_advancement();
    this->advance();

    if(this->current_token.value().type != TokenType::Identifier) {
        return *result.failure(InvalidSyntaxError(
            this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
            "Expected class name"
        ));
    }

    Token class_name_token = this->current_token.value();
    result.register_advancement();
    this->advance();

    if(this->current_token.value().type != TokenType::LCurly) {
        return *result.failure(InvalidSyntaxError(
            this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
            "Expected '{'"
        ));
    }

    result.register_advancement();
    this->advance();

    BaseNode* body = result.reg(this->statements());
    if(result.should_return()) return result;

    for(BaseNode* statement : static_cast<StatementsNode*>(body)->statements) {
        if(statement->type == NodeType::FunctionDefinitionNode) {
            static_cast<FunctionDefinitionNode*>(statement)->should_add_to_table = false;
        }
    }

    if(this->current_token.value().type != TokenType::RCurly) {
        return *result.failure(InvalidSyntaxError(
            this->current_token.value().start_pos.value(), this->current_token.value().end_pos.value(),
            "Expected '}'"
        ));
    }

    result.register_advancement();
    this->advance();

    return *result.success(new ClassDefinitionNode(class_name_token, body, start_pos, this->current_token.value().end_pos.value()));
}
