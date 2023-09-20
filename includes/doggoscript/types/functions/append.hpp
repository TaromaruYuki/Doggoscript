#pragma once

#include "../function.hpp"

struct AppendFunction : public BuiltInFunction {
    AppendFunction() : BuiltInFunction("append", {"list", "value"}, AppendFunction::call) {}

    static RuntimeResult call(std::vector<Object*> args) {
        RuntimeResult result;

        if(args[0]->type != ObjectType::List)
            return *result.failure(ArgumentError(
                *args[0]->start_pos, *args[0]->end_pos,
                "First argument must be a list"
        ));

        List* list = dynamic_cast<List*>(args[0]);
        list->elements.push_back(args[1]);

        return *result.success(list);
    }
};
