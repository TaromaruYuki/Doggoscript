#pragma once

#include "error.hpp"
#include "position.hpp"
#include "token.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

struct LexerResult {
    std::optional<std::vector<Token>> tokens;
    std::optional<BaseError>          error;
};

class Lexer {
    Position    position;
    std::string src;
    char        current_character = '\0';

    void advance();

    std::tuple<std::optional<Token>, std::optional<Token>,
               std::optional<BaseError>>
        create_number();

    Token create_power();

    Token create_minus_or_arrow();

    std::tuple<std::optional<Token>, std::optional<BaseError>> create_string();

    Token create_identifier();

    Token create_not_equals();

    Token create_greater_than();

    Token create_less_than();

    std::tuple<std::optional<Token>, std::optional<BaseError>> create_and();

    std::tuple<std::optional<Token>, std::optional<BaseError>> create_or();

    std::tuple<std::optional<Token>, std::optional<BaseError>> create_through();

    std::optional<Token> create_div_or_comment();

  public:
    explicit Lexer(std::string file_name, std::string src) :
        src(std::move(src)), position(std::move(file_name), src) {
        this->advance();
    }

    LexerResult tokenize();
};
