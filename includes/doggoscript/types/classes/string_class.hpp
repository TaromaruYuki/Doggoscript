#pragma once

#include "../class.hpp"

struct Instance; // From doggoscript/types/instance.hpp

struct StringClass : public BuiltInClass {
    std::string value;

    static Instance* new_instance(std::string initial_value);

    explicit StringClass(std::string initial_value);

    std::optional<std::string> to_string() override {
        return this->value;
    }
};