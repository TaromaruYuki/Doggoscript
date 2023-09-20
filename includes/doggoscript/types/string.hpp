#pragma once

#include <tuple>
#include <string>
#include "object.hpp"

struct String;
typedef std::tuple<std::optional<String>, std::optional<BaseError>> StringResult;

struct String : public Object {
    std::string value;
    String() : value("") { this->type = ObjectType::String; }
    explicit String(std::string value) : value(value) { this->type = ObjectType::String; }

    std::string str() {
        return std::format("\"{}\"", this->value);
    }

    std::string print_friendly() {
        return this->value;
    }

    bool is_true() override {
        return !this->value.empty();
    }

    StringResult operator+(const String& other) const {
        return {String(this->value + other.value), std::nullopt};
    }
};
