#pragma once

#include <optional>
#include <tuple>
#include <unordered_map>
#include "../position.hpp"
#include "../context.hpp"
#include "../error.hpp"


enum class ObjectType {
    Boolean,
    Function,
    List,
    Dict,
    Number,
    Object,
    Statements,
    String,
    Class,
};

const std::unordered_map<ObjectType, std::string> object_type_to_string = {
        {ObjectType::Boolean,    "boolean"},
        {ObjectType::Function,   "function"},
        {ObjectType::List,       "list"},
        {ObjectType::Dict,       "dict"},
        {ObjectType::Number,     "number"},
        {ObjectType::Object,     "object"},
        {ObjectType::Statements, "statements"},
        {ObjectType::String,     "string"},
};

struct Object;

typedef std::tuple<std::optional<Object *>, std::optional<BaseError>> ObjectResult;

struct Object {
    ObjectType type;
    std::optional<Position> start_pos;
    std::optional<Position> end_pos;
    Context *context;

    void set_pos(Position start_pos, Position end_pos) {
        this->start_pos = start_pos;
        this->end_pos = end_pos;
    }

    void set_context(Context *context) {
        this->context = context;
    }

    virtual std::string str() {
        return "<object>";
    }

    virtual std::string print_friendly() {
        return this->str();
    }

    virtual bool is_true() {
        return true;
    }

    virtual ObjectResult operator+(Object &other) {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '+' between '" + object_type_to_string.at(this->type) +
                                        "' and '" + object_type_to_string.at(other.type) + "'!")};
    }

    virtual ObjectResult operator-(Object &other) {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '-' between '" + object_type_to_string.at(this->type) +
                                        "' and  '" + object_type_to_string.at(other.type) + "'!")};
    }

    virtual ObjectResult operator*(Object &other) {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '*' between '" + object_type_to_string.at(this->type) +
                                        "' and  '" + object_type_to_string.at(other.type) + "'!")};
    }

    virtual ObjectResult operator/(Object &other) {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '/' between '" + object_type_to_string.at(this->type) +
                                        "' and  '" + object_type_to_string.at(other.type) + "'!")};
    }

    virtual ObjectResult logical_and(Object &other) {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '&&' between '" + object_type_to_string.at(this->type) +
                                        "' and  '" + object_type_to_string.at(other.type) + "'!")};
    }

    virtual ObjectResult logical_or(Object &other) {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '||' between '" + object_type_to_string.at(this->type) +
                                        "' and  '" + object_type_to_string.at(other.type) + "'!")};
    }

    virtual ObjectResult logical_not() {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '!' on type '" + object_type_to_string.at(this->type) +
                                        "'!")};
    }

    virtual ObjectResult operator<(Object &other) {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '<' on type '" + object_type_to_string.at(this->type) +
                                        "'!")};
    }

    virtual ObjectResult operator<=(Object &other) {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '<=' on type '" + object_type_to_string.at(this->type) +
                                        "'!")};
    }

    virtual ObjectResult operator>(Object &other) {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '>' on type '" + object_type_to_string.at(this->type) +
                                        "'!")};
    }

    virtual ObjectResult operator>=(Object &other) {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '>=' on type '" + object_type_to_string.at(this->type) +
                                        "'!")};
    }

    virtual ObjectResult operator==(Object &other) {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '=' on type '" + object_type_to_string.at(this->type) +
                                        "'!")};
    }

    virtual ObjectResult operator!=(Object &other) {
        return {std::nullopt, TypeError(this->start_pos.value(), this->end_pos.value(),
                                        "Cannot do operation '!=' on type '" + object_type_to_string.at(this->type) +
                                        "'!")};
    }
};