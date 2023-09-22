#pragma once

#include <tuple>
#include "object.hpp"
#include "boolean.hpp"

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

    ObjectResult operator+(Object &other) override {
        if (other.type == ObjectType::Number) {
            auto *other_number = dynamic_cast<Number *>(&other);

            return {new Number(this->value + other_number->value), std::nullopt};
        }

        return Object::operator+(other);
    }

    ObjectResult operator-(Object &other) override {
        if (other.type == ObjectType::Number) {
            auto *other_number = dynamic_cast<Number *>(&other);

            return {new Number(this->value - other_number->value), std::nullopt};
        }

        return Object::operator-(other);
    }

    ObjectResult operator*(Object &other) override {
        if (other.type == ObjectType::Number) {
            auto *other_number = dynamic_cast<Number *>(&other);

            return {new Number(this->value * other_number->value), std::nullopt};
        }

        return Object::operator*(other);
    }

    ObjectResult operator/(Object &other) override {
        if (other.type == ObjectType::Number) {
            auto *other_number = dynamic_cast<Number *>(&other);

            if (other_number->value == 0)
                return {std::nullopt,
                        ArithmeticError(this->start_pos.value(), this->end_pos.value(), "Division by zero")};

            return {new Number(this->value / other_number->value), std::nullopt};
        }

        return Object::operator/(other);
    }

    ObjectResult operator<(Object &other) override {
        if (other.type == ObjectType::Number) {
            auto *other_number = dynamic_cast<Number *>(&other);

            return {new Boolean(this->value < other_number->value), std::nullopt};
        }

        return Object::operator<(other);
    }

    ObjectResult operator>(Object &other) override {
        if (other.type == ObjectType::Number) {
            auto *other_number = dynamic_cast<Number *>(&other);

            return {new Boolean(this->value > other_number->value), std::nullopt};
        }

        return Object::operator>(other);
    }

    ObjectResult operator<=(Object &other) override {
        if (other.type == ObjectType::Number) {
            auto *other_number = dynamic_cast<Number *>(&other);

            return {new Boolean(this->value <= other_number->value), std::nullopt};
        }

        return Object::operator<=(other);
    }

    ObjectResult operator>=(Object &other) override {
        if (other.type == ObjectType::Number) {
            auto *other_number = dynamic_cast<Number *>(&other);

            return {new Boolean(this->value >= other_number->value), std::nullopt};
        }

        return Object::operator>=(other);
    }

    ObjectResult operator==(Object &other) override {
        if (other.type == ObjectType::Number) {
            auto *other_number = dynamic_cast<Number *>(&other);

            return {new Boolean(this->value == other_number->value), std::nullopt};
        }

        return Object::operator==(other);
    }

    ObjectResult operator!=(Object &other) override {
        if (other.type == ObjectType::Number) {
            auto *other_number = dynamic_cast<Number *>(&other);

            return {new Boolean(this->value != other_number->value), std::nullopt};
        }

        return Object::operator!=(other);
    }

    ObjectResult logical_and(Object &other) override {
        if (other.type == ObjectType::Number) {
            auto *other_number = dynamic_cast<Number *>(&other);

            return {new Boolean(this->value && other_number->value), std::nullopt};
        }

        return Object::logical_and(other);
    }

    ObjectResult logical_or(Object &other) override {
        if (other.type == ObjectType::Number) {
            auto *other_number = dynamic_cast<Number *>(&other);

            return {new Boolean(this->value || other_number->value), std::nullopt};
        }

        return Object::logical_or(other);
    }

    ObjectResult logical_not() override {
        return {new Boolean(this->value), std::nullopt};
    }
};