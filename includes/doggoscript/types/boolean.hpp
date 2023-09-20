#pragma once

#include <tuple>
#include "object.hpp"

struct Boolean;

typedef std::tuple<std::optional<Boolean>, std::optional<BaseError>> BooleanResult;

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

    [[nodiscard]] BooleanResult equals(const Boolean& other) const {
        return {Boolean(this->value == other.value), std::nullopt};
    }

    [[nodiscard]] BooleanResult logical_and(const Boolean& other) const {
        return {Boolean(this->value && other.value), std::nullopt};
    }

    [[nodiscard]] BooleanResult logical_or(const Boolean& other) const {
        return {Boolean(this->value || other.value), std::nullopt};
    }

    [[nodiscard]] BooleanResult logical_not() const {
        return {Boolean(!this->value), std::nullopt};
    }
};