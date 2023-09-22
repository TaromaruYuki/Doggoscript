#pragma once

#include <tuple>
#include <string>
#include "object.hpp"

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

    ObjectResult operator+(Object &other) override {
        if (other.type == ObjectType::String) {
            auto *other_string = dynamic_cast<String *>(&other);

            return {new String(this->value + other_string->value), std::nullopt};
        }

        return Object::operator+(other);
    }
};
