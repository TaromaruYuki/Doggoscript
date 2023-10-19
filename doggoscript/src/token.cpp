#include <doggoscript/token.hpp>

Token::Token(TokenType type, std::optional<std::string> value,
             std::optional<Position> start_pos,
             std::optional<Position> end_pos) :
    type(type) {
    if(value.has_value()) this->value = value.value();
    else
        this->value = "";

    if(start_pos.has_value()) {
        this->start_pos = Position(start_pos.value());
        this->end_pos   = Position(start_pos.value());
        this->end_pos.value().advance();
    }

    if(end_pos.has_value()) { this->end_pos = Position(end_pos.value()); }
}

bool Token::is_keyword(const std::string& keyword) {
    return this->type == TokenType::Keyword &&
           std::find(keywords.begin(), keywords.end(), this->value) !=
               keywords.end() &&
           this->value == keyword;
}

std::string Token::str() const {
    std::stringstream result;
    //    result << type_to_str[this->type]
    result << type_to_str.at(this->type);

    if(!this->value.empty()) { result << ":" << this->value; }

    return result.str();
}
