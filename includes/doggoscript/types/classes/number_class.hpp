#pragma once

#include "../class.hpp"

struct Instance; // From doggoscript/types/instance.hpp

struct NumberClass : public BuiltInClass {
    double value;

    static Instance *new_instance(double initial_value);

    explicit NumberClass(double initial_value);

    std::optional<std::string> to_string() override {
        return std::format("{}", this->value);
    }
};