#pragma once

#include "../function.hpp"

struct LenFunction : public BuiltInFunction {
    LenFunction() : BuiltInFunction("len", {"value"}, LenFunction::call) {}

    static RuntimeResult call(std::vector<Object *> args) {
        RuntimeResult result;

        if (args[0]->type == ObjectType::List) {
            List *list = static_cast<List *>(args[0]);
            return *result.success(new Number(list->elements.size()));
        } else if (args[0]->type == ObjectType::String) {
            String *string = static_cast<String *>(args[0]);
            return *result.success(new Number(string->value.size()));
        }

        return *result.failure(ArgumentError(
                *args[0]->start_pos, *args[0]->end_pos,
                "Argument must be a list or string"
        ));
    }
};
