#pragma once

#include "../function.hpp"

struct PrintFunction : public BuiltInFunction {
    PrintFunction() : BuiltInFunction("print", {"value"}, PrintFunction::call) {}

    static RuntimeResult call(std::vector<Object*> args) {
        RuntimeResult result;

        std::cout << args[0]->print_friendly() << std::endl;

        return *result.success(nullptr);
    }
};
