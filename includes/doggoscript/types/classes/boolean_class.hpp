#pragma once

#include "../class.hpp"

struct Instance; // From doggoscript/types/instance.hpp

struct BooleanClass : public BuiltInClass {
    bool value;

    static Instance *new_instance(bool initial_value);

    explicit BooleanClass(bool initial_value);

    std::optional<std::string> to_string() override {
        return value ? "true" : "false";
    }
};