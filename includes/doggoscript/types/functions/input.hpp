#pragma once

#include "../function.hpp"

struct InputFunction : public BuiltInFunction {
    InputFunction() : BuiltInFunction("input", {}, InputFunction::call) {}

    static RuntimeResult call(std::vector<Object *> args) {
        RuntimeResult result;

        std::string input;
        if (!std::getline(std::cin, input)) {
            return *result.failure(BaseError(
                    *args[0]->start_pos, *args[0]->end_pos,
                    "Runtime Error",
                    "Failed to read input from stdin"
            ));
        }

        return *result.success(new String(input));
    }
};
