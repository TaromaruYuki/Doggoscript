#pragma once

#include <tuple>
#include "object.hpp"

struct Boolean : public Object {
    bool value;

    Boolean() : value(false) { this->type = ObjectType::Boolean; }

    explicit Boolean(std::string value) : value(value == "true") { this->type = ObjectType::Boolean; }

    explicit Boolean(bool value) : value(value) { this->type = ObjectType::Boolean; }

    std::string str() override {
        return std::format("{}", this->value);
    }

    bool is_true() override {
        return this->value;
    }

    ObjectResult operator!=(Object &other) override {
        if (other.type == ObjectType::Boolean) {
            auto *other_boolean = dynamic_cast<Boolean *>(&other);

            return {new Boolean(this->value != other_boolean->value), std::nullopt};
        }

        return Object::operator!=(other);
    }

    ObjectResult logical_and(Object &other) override {
        if (other.type == ObjectType::Boolean) {
            auto *other_boolean = dynamic_cast<Boolean *>(&other);

            return {new Boolean(this->value && other_boolean->value), std::nullopt};
        }

        return Object::logical_and(other);
    }

    ObjectResult logical_or(Object &other) override {
        if (other.type == ObjectType::Boolean) {
            auto *other_boolean = dynamic_cast<Boolean *>(&other);

            return {new Boolean(this->value || other_boolean->value), std::nullopt};
        }

        return Object::logical_and(other);
    }

    ObjectResult logical_not() override {
        return {new Boolean(!this->value), std::nullopt};
    }
};