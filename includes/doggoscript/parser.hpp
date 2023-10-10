#pragma once

#include <utility>
#include <vector>
#include <optional>
#include <functional>
#include "token.hpp"
#include "nodes.hpp"
#include "error.hpp"

struct ParseResult {
    BaseNode *node = nullptr;
    std::optional<BaseError> error = std::nullopt;
    int advance_count = 0;
    int to_reverse_count = 0;

    BaseNode *reg(ParseResult result) {
        this->advance_count += result.advance_count;

        if (result.error.has_value())
            this->error = result.error;

        return result.node;
    }

    void register_advancement() {
        this->advance_count++;
    }

    std::optional<BaseNode *> try_reg(ParseResult result) {
        if (result.error.has_value()) {
            this->to_reverse_count = result.advance_count;
            return std::nullopt;
        }

        return this->reg(result);
    }

    ParseResult *success(BaseNode *node) {
        this->node = node;

        return this;
    }

    ParseResult *failure(BaseError error) {
        if (!this->error.has_value() || this->advance_count == 0)
            this->error = error;

        return this;
    }

    bool should_return() const {
        return this->error.has_value();
    }
};

typedef ParseResult(ParserFunction)();

class Parser {
    std::vector<Token> tokens;
    int index = -1;
    std::optional<Token> current_token;

    void advance();

    void reverse(int amount = 1);

    void update_current_token();

    ParseResult statements();

    ParseResult statement();

    ParseResult expr();

    ParseResult comp_expr();

    ParseResult arith_expr();

    ParseResult term();

    ParseResult factor();

    ParseResult power();

    ParseResult call();

    ParseResult atom();

    ParseResult list_expr();

    ParseResult dict_expr();

    ParseResult if_expr();

    ParseResult for_expr();

    ParseResult while_expr();

    ParseResult func_def();

    ParseResult class_def();

    ParseResult bin_op(const std::function<ParserFunction> &func, std::vector<TokenType> ops,
                       std::optional<std::function<ParserFunction>> func2 = std::nullopt);

public:

    explicit Parser(std::vector<Token> tokens)
            : tokens(std::move(tokens)) {
        this->advance();
    }

    ParseResult parse();

};
