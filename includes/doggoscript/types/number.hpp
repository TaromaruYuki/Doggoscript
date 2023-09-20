#pragma once

#include <tuple>
#include "object.hpp"
#include "boolean.hpp"

struct Number;

typedef std::tuple<std::optional<Number>, std::optional<BaseError>> NumberResult;

struct Number : public Object {
    double value;
    Number() : value(0) { this->type = ObjectType::Number; }
    explicit Number(std::string value) : value(std::stod(value)) { this->type = ObjectType::Number; }
    explicit Number(double value) : value(value) { this->type = ObjectType::Number; }

    std::string str() {
        return std::format("{}", this->value);
    }

    bool is_true() override {
        return this->value != 0;
    }

    NumberResult operator+(const Number& other) const {
        return {Number(this->value + other.value), std::nullopt};
    }

    NumberResult operator-(const Number& other) const {
        return {Number(this->value - other.value), std::nullopt};
    }

    NumberResult operator*(const Number& other) const {
        return {Number(this->value * other.value), std::nullopt};
    }

    NumberResult operator/(const Number& other) const {
        if(other.value == 0)
            return {std::nullopt, ArithmeticError(this->start_pos.value(), this->end_pos.value(), "Division by zero")};

        return {Number(this->value / other.value), std::nullopt};
    }

    BooleanResult operator<(const Number& other) const {
        return {Boolean(this->value < other.value), std::nullopt};
    }

    BooleanResult operator>(const Number& other) const {
        return {Boolean(this->value > other.value), std::nullopt};
    }

    BooleanResult operator<=(const Number& other) const {
        return {Boolean(this->value <= other.value), std::nullopt};
    }

    BooleanResult operator>=(const Number& other) const {
        return {Boolean(this->value >= other.value), std::nullopt};
    }

    BooleanResult operator==(const Number& other) const {
        return {Boolean(this->value == other.value), std::nullopt};
    }

    BooleanResult operator!=(const Number& other) const {
        return {Boolean(this->value != other.value), std::nullopt};
    }

    BooleanResult logical_and(const Number &other) const {
        return {Boolean(this->value && other.value), std::nullopt};
    }

    BooleanResult logical_or(const Number &other) const {
        return {Boolean(this->value || other.value), std::nullopt};
    }

    BooleanResult logical_not() const {
        return {Boolean(!this->value), std::nullopt};
    }
};