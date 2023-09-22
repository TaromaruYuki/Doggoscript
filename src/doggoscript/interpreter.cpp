#include "../../includes/doggoscript/interpreter.hpp"
#include "../../includes/doggoscript/types/types.hpp"

RuntimeResult Interpreter::visit(BaseNode *node, Context &context) {
    switch (node->type) {
        case NodeType::NumberNode:
            return this->visit_NumberNode(static_cast<NumberNode *>(node), context);
        case NodeType::BinaryOperationNode:
            return this->visit_BinaryOperationNode(static_cast<BinaryOperationNode *>(node), context);
        case NodeType::UnaryOperationNode:
            return this->visit_UnaryOperationNode(static_cast<UnaryOperationNode *>(node), context);
        case NodeType::StatementsNode:
            return this->visit_StatementsNode(static_cast<StatementsNode *>(node), context);
        case NodeType::BooleanNode:
            return this->visit_BooleanNode(static_cast<BooleanNode *>(node), context);
        case NodeType::VariableAssignmentNode:
            return this->visit_VariableAssignmentNode(static_cast<VariableAssignmentNode *>(node), context);
        case NodeType::VariableAccessNode:
            return this->visit_VariableAccessNode(static_cast<VariableAccessNode *>(node), context);
        case NodeType::VariableReassignmentNode:
            return this->visit_VariableReassignmentNode(static_cast<VariableReassignmentNode *>(node), context);
        case NodeType::StringNode:
            return this->visit_StringNode(static_cast<StringNode *>(node), context);
        case NodeType::ListNode:
            return this->visit_ListNode(static_cast<ListNode *>(node), context);
        case NodeType::DictNode:
            return this->visit_DictNode(static_cast<DictNode *>(node), context);
        case NodeType::FunctionDefinitionNode:
            return this->visit_FunctionDefinitionNode(static_cast<FunctionDefinitionNode *>(node), context);
        case NodeType::IdentifierNode:
            return this->visit_IdentifierNode(static_cast<IdentifierNode *>(node), context);
        case NodeType::CallNode:
            return this->visit_CallNode(static_cast<CallNode *>(node), context);
        case NodeType::ReturnNode:
            return this->visit_ReturnNode(static_cast<ReturnNode *>(node), context);
        case NodeType::ContinueNode:
            return this->visit_ContinueNode(static_cast<ContinueNode *>(node), context);
        case NodeType::BreakNode:
            return this->visit_BreakNode(static_cast<BreakNode *>(node), context);
        case NodeType::IfNode:
            return this->visit_IfNode(static_cast<IfNode *>(node), context);
        case NodeType::ForNode:
            return this->visit_ForNode(static_cast<ForNode *>(node), context);
        case NodeType::WhileNode:
            return this->visit_WhileNode(static_cast<WhileNode *>(node), context);
        default: {
            std::cerr << "Could not visit node '" << node_type_to_str.at(node->type) << "'" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

RuntimeResult Interpreter::visit_NumberNode(NumberNode *node, Context &context) {
    auto *number = new Number(node->token->value);
    number->set_pos(*node->start_pos, *node->end_pos);
    number->set_context(&context);

    return *RuntimeResult().success(number);
}

RuntimeResult Interpreter::visit_BooleanNode(BooleanNode *node, Context &context) {
    auto *boolean = new Boolean(node->token->value);
    boolean->set_pos(*node->start_pos, *node->end_pos);
    boolean->set_context(&context);

    return *RuntimeResult().success(boolean);
}

RuntimeResult Interpreter::visit_StringNode(StringNode *node, Context &context) {
    auto *string = new String(node->token->value);
    string->set_pos(*node->start_pos, *node->end_pos);
    string->set_context(&context);

    return *RuntimeResult().success(string);
}

RuntimeResult Interpreter::visit_ListNode(ListNode *node, Context &context) {
    RuntimeResult result;
    std::vector<Object *> elements;

    for (BaseNode *element: node->elements) {
        std::optional<Object *> reg_res = result.reg(this->visit(element, context));

        if (result.should_return())
            return result;

        if (reg_res.has_value())
            elements.push_back(reg_res.value());
    }

    auto *list = new List(elements);
    list->set_pos(*node->start_pos, *node->end_pos);
    list->set_context(&context);

    return *result.success(list);
}

RuntimeResult Interpreter::visit_BinaryOperationNode(BinaryOperationNode *node, Context &context) {
    RuntimeResult result;

    std::optional<Object *> left_o_res = result.reg(this->visit(node->lhs, context));
    if (result.should_return())
        return result;

    Object *left_o = left_o_res.value();

    if (left_o->type != ObjectType::Number && left_o->type != ObjectType::Boolean && left_o->type != ObjectType::String)
        return *result.failure(IllegalOperationError(
                *node->start_pos, *node->end_pos,
                "Expected number, boolean, or string"
        ));

    Number *left = static_cast<Number *>(left_o);

    std::optional<Object *> right_o_res = result.reg(this->visit(node->rhs, context));
    if (result.should_return())
        return result;

    Object *right_o = right_o_res.value();

    if (right_o->type != ObjectType::Number && right_o->type != ObjectType::Boolean &&
        right_o->type != ObjectType::String)
        return *result.failure(IllegalOperationError(
                *node->start_pos, *node->end_pos,
                "Expected number, boolean, or string"
        ));

    Number *right = static_cast<Number *>(right_o);

    std::optional<NumberResult> num_result;
    std::optional<BooleanResult> bool_result;
    std::optional<StringResult> str_result;

    switch (node->token->type) {
        case TokenType::Plus: {
            if (left->type == ObjectType::Number && right->type == ObjectType::Number)
                num_result = *left + *right;
            else if (left->type == ObjectType::String && right->type == ObjectType::String) {
                auto *left_string = static_cast<String *>(left_o);
                auto *right_string = static_cast<String *>(right_o);

                str_result = *left_string + *right_string;
            } else
                return *result.failure(IllegalOperationError(
                        *node->start_pos, *node->end_pos,
                        "Operator '+' can only be between two numbers or string."
                ));
        }
            break;
        case TokenType::Minus: {
            if (left->type == ObjectType::Number && right->type == ObjectType::Number)
                num_result = *left - *right;
            else
                return *result.failure(IllegalOperationError(
                        *node->start_pos, *node->end_pos,
                        "Operator '-' can only be between two numbers."
                ));
        }
            break;
        case TokenType::Multiply: {
            if (left->type == ObjectType::Number && right->type == ObjectType::Number)
                num_result = *left * *right;
            else
                return *result.failure(IllegalOperationError(
                        *node->start_pos, *node->end_pos,
                        "Operator '*' can only be between two numbers."
                ));
        }
            break;
        case TokenType::Divide: {
            if (left->type == ObjectType::Number && right->type == ObjectType::Number)
                num_result = *left / *right;
            else
                return *result.failure(IllegalOperationError(
                        *node->start_pos, *node->end_pos,
                        "Operator '/' can only be between two numbers."
                ));
        }
            break;
        case TokenType::LogicalAnd: {
            if (left->type == ObjectType::Number && right->type == ObjectType::Number)
                bool_result = left->logical_and(*right);
            else {
                auto *left_boolean = static_cast<Boolean *>(left_o);
                auto *right_boolean = static_cast<Boolean *>(right_o);

                bool_result = left_boolean->logical_and(*right_boolean);
            }
        }
            break;
        case TokenType::LogicalOr: {
            if (left->type == ObjectType::Number && right->type == ObjectType::Number)
                bool_result = left->logical_or(*right);
            else {
                auto *left_boolean = static_cast<Boolean *>(left_o);
                auto *right_boolean = static_cast<Boolean *>(right_o);

                bool_result = left_boolean->logical_or(*right_boolean);
            }
        }
            break;
        case TokenType::LessThan: {
            if (left->type == ObjectType::Number && right->type == ObjectType::Number)
                bool_result = *left < *right;
            else
                return *result.failure(IllegalOperationError(
                        *node->start_pos, *node->end_pos,
                        "Operator '<' can only be between two numbers."
                ));
        }
            break;
        case TokenType::GreaterThan: {
            if (left->type == ObjectType::Number && right->type == ObjectType::Number)
                bool_result = *left > *right;
            else
                return *result.failure(IllegalOperationError(
                        *node->start_pos, *node->end_pos,
                        "Operator '>' can only be between two numbers."
                ));
        }
            break;
        case TokenType::LessThanEquals: {
            if (left->type == ObjectType::Number && right->type == ObjectType::Number)
                bool_result = *left <= *right;
            else
                return *result.failure(IllegalOperationError(
                        *node->start_pos, *node->end_pos,
                        "Operator '<=' can only be between two numbers."
                ));
        }
            break;
        case TokenType::GreaterThanEquals: {
            if (left->type == ObjectType::Number && right->type == ObjectType::Number)
                bool_result = *left >= *right;
            else
                return *result.failure(IllegalOperationError(
                        *node->start_pos, *node->end_pos,
                        "Operator '>=' can only be between two numbers."
                ));
        }
            break;
        case TokenType::EqualsTo: {
            if (left->type == ObjectType::Number && right->type == ObjectType::Number)
                bool_result = *left == *right;
            else
                return *result.failure(IllegalOperationError(
                        *node->start_pos, *node->end_pos,
                        "Operator '=' can only be between two numbers."
                ));
        }
        case TokenType::NotEquals: {
            if (left->type == ObjectType::Number && right->type == ObjectType::Number)
                bool_result = *left != *right;
            else
                return *result.failure(IllegalOperationError(
                        *node->start_pos, *node->end_pos,
                        "Operator '!=' can only be between two numbers."
                ));
        }
    }

    if (num_result.has_value()) {
        if (std::get<1>(num_result.value()).has_value())
            return *result.failure(std::get<1>(num_result.value()).value());

        Number *num = new Number(std::get<0>(num_result.value()).value());
        num->set_pos(*node->start_pos, *node->end_pos);

        return *result.success(num);
    } else if (str_result.has_value()) {
        if (std::get<1>(str_result.value()).has_value())
            return *result.failure(std::get<1>(str_result.value()).value());

        String *string = new String(std::get<0>(str_result.value()).value());
        string->set_pos(*node->start_pos, *node->end_pos);

        return *result.success(string);
    } else {
        if (std::get<1>(bool_result.value()).has_value())
            return *result.failure(std::get<1>(bool_result.value()).value());

        Boolean *boolean = new Boolean(std::get<0>(bool_result.value()).value());
        boolean->set_pos(*node->start_pos, *node->end_pos);

        return *result.success(boolean);
    }
}

RuntimeResult Interpreter::visit_UnaryOperationNode(UnaryOperationNode *node, Context &context) {
    RuntimeResult result;
    std::optional<Object *> num_o_res = result.reg(this->visit(node->value, context));
    if (result.should_return())
        return result;

    Object *num_o = num_o_res.value();

    if (num_o->type == ObjectType::Number) {
        auto *num = static_cast<Number *>(num_o);

        if (node->token->type == TokenType::Minus) {
            Number neg_1 = Number(-1);
            neg_1.set_context(&context);
            NumberResult res = *num * neg_1;
            if (std::get<1>(res).has_value())
                return *result.failure(std::get<1>(res).value());

            num = new Number(std::get<0>(res).value());
        }

        return *result.success(num);
    } else if (num_o->type == ObjectType::Boolean) {
        auto *boolean = static_cast<Boolean *>(num_o);

        if (node->token->type == TokenType::LogicalNot) {
            BooleanResult res = boolean->logical_not();

            if (std::get<1>(res).has_value())
                return *result.failure(std::get<1>(res).value());

            boolean = new Boolean(std::get<0>(res).value());
        }

        return *result.success(boolean);
    }

    return *result.failure(IllegalOperationError(
            *node->start_pos, *node->end_pos,
            "Expected number or boolean"
    ));
}

RuntimeResult Interpreter::visit_StatementsNode(StatementsNode *node, Context &context) {
    RuntimeResult result;
    std::vector<Object *> values;

    for (BaseNode *statement: node->statements) {
        std::optional<Object *> reg_res = result.reg(this->visit(statement, context));

        if (result.should_return())
            return result;

        if (reg_res.has_value())
            values.push_back(reg_res.value());
    }

    return *result.success(values[values.size() - 1]);
}

RuntimeResult Interpreter::visit_VariableAssignmentNode(VariableAssignmentNode *node, Context &context) {
    RuntimeResult result;

    std::optional<Object *> value = result.reg(this->visit(node->value, context));
    if (result.should_return())
        return result;

    context.symbol_table->set(node->token->value, value.value());
    value.value()->set_pos(*node->start_pos, *node->end_pos);
    value.value()->set_context(&context);

    return *result.success(value.value());
}

RuntimeResult Interpreter::visit_VariableAccessNode(VariableAccessNode *node, Context &context) {
    RuntimeResult result;
    std::string var_name = node->token->value;
    Object *value = context.symbol_table->get(var_name);

    if (value == nullptr)
        return *result.failure(NameError(
                *node->start_pos, *node->end_pos,
                "Variable '" + var_name + "' is not defined"
        ));

    value->set_pos(*node->start_pos, *node->end_pos);
    value->set_context(&context);

    return *result.success(value);
}

RuntimeResult Interpreter::visit_VariableReassignmentNode(VariableReassignmentNode *node, Context &context) {
    RuntimeResult result;
    std::string var_name = node->token->value;
    std::optional<Object *> value = result.reg(this->visit(node->value, context));

    if (result.should_return())
        return result;

    if (context.symbol_table->get(var_name) == nullptr)
        return *result.failure(NameError(
                *node->start_pos, *node->end_pos,
                "Variable '" + var_name + "' is not defined"
        ));

    context.symbol_table->set(var_name, value.value());

    return *result.success(value.value());
}

RuntimeResult Interpreter::visit_FunctionDefinitionNode(FunctionDefinitionNode *node, Context &context) {
    RuntimeResult result;

    std::string func_name = !node->name.has_value() ? "<lambda>" : node->name.value().value;
    std::vector<std::string> arg_names;
    for (Token *arg_name: node->arg_names)
        arg_names.push_back(arg_name->value);

    auto *func = new Function(func_name, node->body_node, arg_names, node->should_auto_return);
    func->set_pos(*node->start_pos, *node->end_pos);

    if (node->name.has_value())
        context.symbol_table->set(func_name, func);

    return *result.success(func);
}

RuntimeResult Interpreter::visit_IdentifierNode(IdentifierNode *node, Context &context) {
    RuntimeResult result;
    Object *value = context.symbol_table->get(node->token->value);

    if (value == nullptr)
        return *result.failure(NameError(
                *node->start_pos, *node->end_pos,
                "Function '" + node->token->value + "' is not defined"
        ));

    auto *func = static_cast<BaseFunction *>(value);

    if (func->func_type == FunctionType::UserDefined) {
        auto *func_copy = new Function(*static_cast<Function *>(func));
        func_copy->set_context(&context);
        func_copy->set_pos(*node->start_pos, *node->end_pos);

        return *result.success(func_copy);
    } else {
        auto *func_copy = new BuiltInFunction(*static_cast<BuiltInFunction *>(func));
        func_copy->set_context(&context);
        func_copy->set_pos(*node->start_pos, *node->end_pos);

        return *result.success(func_copy);
    }
}

RuntimeResult Interpreter::visit_CallNode(CallNode *node, Context &context) {
    RuntimeResult result;
    std::vector<Object *> args;
    std::optional<Object *> func_value = result.reg(this->visit(node->node_to_call, context));
    if (result.should_return())
        return result;

    Object *func_o = func_value.value();

    for (BaseNode *arg_node: node->arg_nodes) {
        std::optional<Object *> arg_value = result.reg(this->visit(arg_node, context));
        if (result.should_return())
            return result;

        args.push_back(arg_value.value());
    }

    if (func_o->type != ObjectType::Function)
        return *result.failure(IllegalOperationError(
                *node->start_pos, *node->end_pos,
                "Expected function"
        ));

    auto *func = static_cast<BaseFunction *>(func_o);
    std::optional<Object *> return_value = result.reg((*func)(args));
    if (result.should_return())
        return result;

    if (return_value.has_value() && return_value != nullptr) {
        Object *return_value_o = return_value.value();
        return_value_o->set_pos(*node->start_pos, *node->end_pos);
        return_value_o->set_context(&context);

        return *result.success(return_value_o);
    }

    return *result.success(nullptr);
}

RuntimeResult Interpreter::visit_ReturnNode(ReturnNode *node, Context &context) {
    RuntimeResult result;

    Object *value;

    if (node->node_to_return != nullptr) {
        std::optional<Object *> val = result.reg(this->visit(node->node_to_return, context));
        if (result.should_return())
            return result;

        value = val.value();
    } else {
        value = nullptr;
    }

    return *result.success_return(value);
}

RuntimeResult Interpreter::visit_ContinueNode(ContinueNode *node, Context &context) {
    return *RuntimeResult().success_continue();
}

RuntimeResult Interpreter::visit_BreakNode(BreakNode *node, Context &context) {
    return *RuntimeResult().success_break();
}

RuntimeResult Interpreter::visit_IfNode(IfNode *node, Context &context) {
    RuntimeResult result;
    Context if_context("");
    auto *if_symbol_table = new SymbolTable(context.symbol_table);
    if_context.symbol_table = if_symbol_table;

    for (auto &case_: node->cases) {
        std::optional<Object *> condition = result.reg(this->visit(std::get<0>(case_), if_context));
        if (result.should_return())
            return result;

        if (condition.has_value() && condition.value()->is_true()) {
            std::optional<Object *> value = result.reg(this->visit(std::get<1>(case_), if_context));
            if (result.should_return())
                return result;

            if (std::get<2>(case_))
                return *result.success(value.value());

            return *result.success(nullptr);
        }
    }

    if (node->else_case.has_value()) {
        auto else_ = node->else_case.value();
        std::optional<Object *> value = result.reg(this->visit(std::get<0>(else_), if_context));
        if (result.should_return())
            return result;

        if (std::get<1>(else_))
            return *result.success(value.value());
    }

    return *result.success(nullptr);
}

RuntimeResult Interpreter::visit_ForNode(ForNode *node, Context &context) {
    RuntimeResult result;
    Context for_context("");
    auto *for_symbol_table = new SymbolTable(context.symbol_table);
    for_context.symbol_table = for_symbol_table;

    std::optional<Object *> start_value_o = result.reg(this->visit(node->start_value, for_context));
    if (result.should_return())
        return result;

    std::optional<Object *> end_value_o = result.reg(this->visit(node->end_value, for_context));
    if (result.should_return())
        return result;

    std::optional<Object *> step_value_o;

    if (node->step_value != nullptr) {
        step_value_o = result.reg(this->visit(node->step_value, for_context));
        if (result.should_return())
            return result;
    } else {
        step_value_o = new Number(1);
    }

    if (start_value_o.value()->type != ObjectType::Number)
        return *result.failure(IllegalOperationError(
                *node->start_pos, *node->end_pos,
                "Expected number for start value"
        ));

    if (end_value_o.value()->type != ObjectType::Number)
        return *result.failure(IllegalOperationError(
                *node->start_pos, *node->end_pos,
                "Expected number for end value"
        ));

    if (step_value_o.value()->type != ObjectType::Number)
        return *result.failure(IllegalOperationError(
                *node->start_pos, *node->end_pos,
                "Expected number for step value"
        ));

    auto *start_value = static_cast<Number *>(start_value_o.value());
    auto *end_value = static_cast<Number *>(end_value_o.value());
    auto *step_value = static_cast<Number *>(step_value_o.value());

    double i = start_value->value;

    bool (*cond)(double, double);

    if (step_value->value >= 0) {
        cond = [](double i, double end) { return i < end; };
    } else {
        cond = [](double i, double end) { return i > end; };
    }

    while (cond(i, end_value->value)) {
        for_symbol_table->set(node->token->value, new Number(i));
        i += step_value->value;

        std::optional<Object *> value = result.reg(this->visit(node->body, for_context));
        if (result.should_return())
            return result;

        if (result.loop_should_continue)
            continue;

        if (result.loop_should_break)
            break;
    }

    return *result.success(nullptr);
}

RuntimeResult Interpreter::visit_WhileNode(WhileNode *node, Context &context) {
    RuntimeResult result;
    Context while_context("");
    auto *while_symbol_table = new SymbolTable(context.symbol_table);
    while_context.symbol_table = while_symbol_table;

    while (true) {
        std::optional<Object *> condition = result.reg(this->visit(node->condition, while_context));
        if (result.should_return())
            return result;

        if (!condition.value()->is_true())
            break;

        std::optional<Object *> value = result.reg(this->visit(node->body, while_context));
        if (result.should_return() && !result.loop_should_continue && !result.loop_should_break)
            return result;

        if (result.loop_should_continue)
            continue;

        if (result.loop_should_break)
            break;
    }

    return *result.success(nullptr);
}

RuntimeResult Interpreter::visit_DictNode(DictNode *node, Context &context) {
    RuntimeResult result;
    std::vector<std::tuple<Object *, Object *>> elements;

    for (auto &element: node->elements) {
        std::optional<Object *> key = result.reg(this->visit(std::get<0>(element), context));
        if (result.should_return())
            return result;

        std::optional<Object *> value = result.reg(this->visit(std::get<1>(element), context));
        if (result.should_return())
            return result;

        elements.emplace_back(key.value(), value.value());
    }

    auto *dict = new Dict(elements);
    dict->set_pos(*node->start_pos, *node->end_pos);
    dict->set_context(&context);

    return *result.success(dict);
}


