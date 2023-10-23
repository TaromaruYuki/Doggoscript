#pragma once

#include "../class.hpp"
#include "../instance.hpp"

struct ErrorClass : public BuiltInClass {
    std::string error_str;

    static Instance* new_instance(BaseError initial_value);

    explicit ErrorClass(BaseError error);

    std::optional<std::string> to_string() override {
        return error_str;
    }
};