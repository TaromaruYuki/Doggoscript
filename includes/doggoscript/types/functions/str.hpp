#pragma once

#include "../function.hpp"

struct StrFunction : public BuiltInFunction {
    StrFunction() : BuiltInFunction("str", {"value"}, StrFunction::call) {}

    static RuntimeResult call(std::vector<Object *> args) {
        RuntimeResult result;

        return *result.success(new String(args[0]->print_friendly()));
    }
};
