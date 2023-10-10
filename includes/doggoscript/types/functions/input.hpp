#pragma once

#include "../function.hpp"
#include "../classes/string_class.hpp"

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

//        return *result.success(new String(input));
        return *result.success(reinterpret_cast<Object *>(StringClass::new_instance(input)));
    }
};
