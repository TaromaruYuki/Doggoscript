#pragma once

#include "../function.hpp"

struct ExtendFunction : public BuiltInFunction {
    ExtendFunction() : BuiltInFunction("extend", {"list_1", "list_2"}, ExtendFunction::call) {}

    static RuntimeResult call(std::vector<Object *> args) {
        RuntimeResult result;

        if (args[0]->type != ObjectType::List)
            return *result.failure(ArgumentError(
                    *args[0]->start_pos, *args[0]->end_pos,
                    "First argument must be a list"
            ));

        if (args[1]->type != ObjectType::List)
            return *result.failure(ArgumentError(
                    *args[1]->start_pos, *args[1]->end_pos,
                    "Second argument must be a list"
            ));

        List *list_1 = dynamic_cast<List *>(args[0]);
        List *list_2 = dynamic_cast<List *>(args[1]);

        for (Object *item: list_2->elements)
            list_1->elements.push_back(item);

        return *result.success(list_1);
    }
};
