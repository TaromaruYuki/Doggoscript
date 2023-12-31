#pragma once

#include "../classes/string_class.hpp"
#include "../function.hpp"

struct StrFunction : public BuiltInFunction {
    StrFunction() : BuiltInFunction("str", { "value" }, StrFunction::call) {}

    static RuntimeResult call(std::vector<Object*> args) {
        return *RuntimeResult().success(
            StringClass::new_instance(args[0]->print_friendly()));
    }
};
