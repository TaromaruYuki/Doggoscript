#include <doggoscript/lexer.hpp>

void Lexer::advance() {
    if(this->position.index < this->src.size()) {
        this->current_character = src[this->position.index];
    } else {
        this->current_character = '\0';
    }

    this->position.advance(this->current_character);
}

LexerResult Lexer::tokenize() {
    std::vector<Token> tokens;

    while(this->current_character != '\0') {
        switch(this->current_character) {
            case ' ':
            case '\t': this->advance(); break;
            case '\n':
            case ';':
                tokens.emplace_back(TokenType::NewLine, std::nullopt,
                                    Position(this->position), std::nullopt);
                this->advance();
                break;
            case '+':
                tokens.emplace_back(TokenType::Plus, std::nullopt,
                                    Position(this->position), std::nullopt);
                this->advance();
                break;
            case '-': tokens.push_back(this->create_minus_or_arrow()); break;
            case '*': tokens.push_back(this->create_power()); break;
            case '/': {
                auto token = this->create_div_or_comment();
                if(token.has_value()) { tokens.push_back(token.value()); }
            } break;
            case '(':
                tokens.emplace_back(TokenType::LParen, std::nullopt,
                                    Position(this->position), std::nullopt);
                this->advance();
                break;
            case ')':
                tokens.emplace_back(TokenType::RParen, std::nullopt,
                                    Position(this->position), std::nullopt);
                this->advance();
                break;
            case '{':
                tokens.emplace_back(TokenType::LCurly, std::nullopt,
                                    Position(this->position), std::nullopt);
                this->advance();
                break;
            case '}':
                tokens.emplace_back(TokenType::RCurly, std::nullopt,
                                    Position(this->position), std::nullopt);
                this->advance();
                break;
            case '[':
                tokens.emplace_back(TokenType::LSquare, std::nullopt,
                                    Position(this->position), std::nullopt);
                this->advance();
                break;
            case ']':
                tokens.emplace_back(TokenType::RSquare, std::nullopt,
                                    Position(this->position), std::nullopt);
                this->advance();
                break;
            case '$': {
                this->advance();
                Token identifier = this->create_identifier();
                identifier.type  = TokenType::Variable;
                identifier.value = "$" + identifier.value;
                tokens.push_back(identifier);
            } break;
            case '=':
                tokens.emplace_back(TokenType::EqualsTo, std::nullopt,
                                    Position(this->position), std::nullopt);
                this->advance();
                break;
            case '!': tokens.push_back(this->create_not_equals()); break;
            case '>': tokens.push_back(this->create_greater_than()); break;
            case '<': tokens.push_back(this->create_less_than()); break;
            case '.': {
                auto token = this->create_through();
                if(std::get<1>(token).has_value()) {
                    return { .error = std::get<1>(token).value() };
                }

                tokens.push_back(std::get<0>(token).value());
            } break;
            case ',':
                tokens.emplace_back(TokenType::Comma, std::nullopt,
                                    Position(this->position), std::nullopt);
                this->advance();
                break;
            case '&': {
                auto token = this->create_and();
                if(std::get<1>(token).has_value()) {
                    return { .error = std::get<1>(token).value() };
                }

                tokens.push_back(std::get<0>(token).value());
            } break;
            case '|': {
                auto token = this->create_or();
                if(std::get<1>(token).has_value()) {
                    return { .error = std::get<1>(token).value() };
                }

                tokens.push_back(std::get<0>(token).value());
            } break;
            case '"': {
                auto token = this->create_string();
                if(std::get<1>(token).has_value()) {
                    return { .error = std::get<1>(token).value() };
                }

                tokens.push_back(std::get<0>(token).value());
            } break;
            default: {
                if(std::isalpha(this->current_character) ||
                   this->current_character == '_') {
                    tokens.push_back(this->create_identifier());
                } else if(isdigit(this->current_character)) {
                    auto num = this->create_number();
                    if(std::get<2>(num).has_value()) {
                        return { .error = std::get<2>(num).value() };
                    }

                    tokens.push_back(std::get<0>(num).value());

                    if(std::get<1>(num).has_value()) {
                        tokens.push_back(std::get<1>(num).value());
                    }
                } else {
                    Position pos_start(this->position);
                    char     character = this->current_character;
                    this->advance();
                    return { .error = IllegalCharacterError(
                                 pos_start, this->position, character) };
                }
            } break;
        }
    }

    tokens.emplace_back(TokenType::FileEnd, std::nullopt, this->position,
                        std::nullopt);

    return { .tokens = tokens };
}

std::tuple<std::optional<Token>, std::optional<Token>, std::optional<BaseError>>
    Lexer::create_number() {
    std::string          num;
    uint8_t              dot_count = 0;
    Position             start_pos(this->position);
    std::optional<Token> through_token;
    char                 prev_character = '\0';

    while(
        this->current_character != '\0' &&
        (isdigit(this->current_character) || this->current_character == '.')) {
        if(this->current_character == '.') {
            if(dot_count == 1) {
                if(prev_character == '.') {
                    through_token = Token(TokenType::Through, std::nullopt,
                                          start_pos, Position(this->position));
                    this->advance();
                    num.pop_back();
                    dot_count = 0;
                    break;
                } else {
                    return { std::nullopt, std::nullopt,
                             IllegalCharacterError(start_pos, this->position,
                                                   '.') };
                }
                break;
            }
            dot_count += 1;
        }

        num.push_back(this->current_character);
        prev_character = this->current_character;
        this->advance();
    }

    Token token(TokenType::Int, num, start_pos, Position(this->position));

    if(dot_count == 1) { token.type = TokenType::Float; }

    return { token, through_token, std::nullopt };
}

Token Lexer::create_power() {
    Position start_pos(this->position);
    this->advance();

    if(this->current_character == '*') {
        this->advance();
        return { TokenType::Power, std::nullopt, start_pos,
                 Position(this->position) };
    }

    return { TokenType::Multiply, std::nullopt, start_pos,
             Position(this->position) };
}

std::tuple<std::optional<Token>, std::optional<BaseError>>
    Lexer::create_string() {
    std::string str;
    Position    start_pos(this->position);
    bool        escape_character = false;
    this->advance();

    std::unordered_map<char, char> escape_characters = {
        { 'n', '\n' },
        { 't', '\t' },
        { 'r', '\r' },
    };

    while(this->current_character != '\0' &&
          (this->current_character != '"' || escape_character)) {
        if(escape_character) {
            if(escape_characters.find(this->current_character) !=
               escape_characters.end()) {
                str.push_back(escape_characters[this->current_character]);
            } else {
                str.push_back(this->current_character);
            }
            escape_character = false;
        } else {
            if(this->current_character == '\\') {
                escape_character = true;
            } else {
                str.push_back(this->current_character);
            }
        }

        this->advance();
    }

    if(this->current_character != '"') {
        return { std::nullopt,
                 ExpectedCharacterError(start_pos, this->position, '"') };
    }

    this->advance();

    return { Token(TokenType::String, str, start_pos, Position(this->position)),
             std::nullopt };
}

Token Lexer::create_identifier() {
    std::string identifier;
    Position    start_pos(this->position);

    while(this->current_character != '\0' &&
          (std::isalnum(this->current_character) ||
           this->current_character == '_')) {
        identifier.push_back(this->current_character);
        this->advance();
    }

    Token token(TokenType::Identifier, identifier, start_pos,
                Position(this->position));

    if(std::find(keywords.begin(), keywords.end(), identifier) !=
       keywords.end()) {
        token.type = TokenType::Keyword;
    }

    return token;
}

Token Lexer::create_not_equals() {
    Position start_pos(this->position);
    this->advance();

    if(this->current_character == '=') {
        this->advance();
        return { TokenType::NotEquals, std::nullopt, start_pos,
                 Position(this->position) };
    }

    return { TokenType::LogicalNot, std::nullopt, start_pos,
             Position(this->position) };
}

Token Lexer::create_greater_than() {
    Position start_pos(this->position);
    this->advance();

    if(this->current_character == '=') {
        this->advance();
        return { TokenType::GreaterThanEquals, std::nullopt, start_pos,
                 Position(this->position) };
    }

    return { TokenType::GreaterThan, std::nullopt, start_pos,
             Position(this->position) };
}

Token Lexer::create_less_than() {
    Position start_pos(this->position);
    this->advance();

    if(this->current_character == '=') {
        this->advance();
        return { TokenType::LessThanEquals, std::nullopt, start_pos,
                 Position(this->position) };
    }

    return { TokenType::LessThan, std::nullopt, start_pos,
             Position(this->position) };
}

std::tuple<std::optional<Token>, std::optional<BaseError>> Lexer::create_and() {
    Position start_pos(this->position);
    this->advance();

    if(this->current_character == '&') {
        this->advance();
        return { Token(TokenType::LogicalAnd, std::nullopt, start_pos,
                       Position(this->position)),
                 std::nullopt };
    }

    return { std::nullopt,
             ExpectedCharacterError(start_pos, this->position, '&') };
}

std::tuple<std::optional<Token>, std::optional<BaseError>> Lexer::create_or() {
    Position start_pos(this->position);
    this->advance();

    if(this->current_character == '|') {
        this->advance();
        return { Token(TokenType::LogicalOr, std::nullopt, start_pos,
                       Position(this->position)),
                 std::nullopt };
    }

    return { std::nullopt,
             ExpectedCharacterError(start_pos, this->position, '|') };
}

std::tuple<std::optional<Token>, std::optional<BaseError>>
    Lexer::create_through() {
    Position start_pos(this->position);
    this->advance();

    if(this->current_character == '.') {
        this->advance();
        return { Token(TokenType::Through, std::nullopt, start_pos,
                       Position(this->position)),
                 std::nullopt };
    }

    return { std::nullopt,
             ExpectedCharacterError(start_pos, this->position, '.') };
}

std::optional<Token> Lexer::create_div_or_comment() {
    Position start_pos(this->position);
    this->advance();

    if(this->current_character == '/') {
        while(this->current_character != '\n' &&
              this->current_character != '\0') {
            this->advance();
        }

        this->advance();
        return std::nullopt;
    }

    return Token(TokenType::Divide, std::nullopt, start_pos,
                 Position(this->position));
}

Token Lexer::create_minus_or_arrow() {
    Position start_pos(this->position);
    this->advance();

    if(this->current_character == '>') {
        this->advance();
        return { TokenType::Arrow, std::nullopt, start_pos,
                 Position(this->position) };
    }

    return { TokenType::Minus, std::nullopt, start_pos,
             Position(this->position) };
}
