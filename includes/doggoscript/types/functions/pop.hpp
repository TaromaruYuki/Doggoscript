#pragma once

#include "../function.hpp"

struct PopFunction : public BuiltInFunction {
    PopFunction() : BuiltInFunction("append", {"list", "index"}, PopFunction::call) {}

    static RuntimeResult call(std::vector<Object*> args) {
        RuntimeResult result;

        if(args[0]->type != ObjectType::List)
            return *result.failure(ArgumentError(
                    *args[0]->start_pos, *args[0]->end_pos,
                    "First argument must be a list"
            ));

        if(args[1]->type != ObjectType::Number)
            return *result.failure(ArgumentError(
                    *args[0]->start_pos, *args[0]->end_pos,
                    "Second argument must be a number"
            ));

        List* list = dynamic_cast<List*>(args[0]);
        Number* index = dynamic_cast<Number*>(args[1]);

        if(index->value < 0 || index->value >= list->elements.size())
            return *result.failure(ArgumentError(
                    *args[0]->start_pos, *args[0]->end_pos,
                    "Index out of bounds"
            ));

        Object* popped = list->elements[index->value];
        list->elements.erase(list->elements.begin() + index->value);

        return *result.success(popped);
    }
};
