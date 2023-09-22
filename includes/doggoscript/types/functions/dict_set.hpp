#pragma once

#include "../function.hpp"

struct DictSetFunction : public BuiltInFunction {
    DictSetFunction() : BuiltInFunction("dict_set", {"dict", "key", "value"}, DictSetFunction::call) {}

    static RuntimeResult call(std::vector<Object *> args) {
        RuntimeResult result;

        if (args[0]->type != ObjectType::Dict)
            return *result.failure(ArgumentError(
                    *args[0]->start_pos, *args[0]->end_pos,
                    "First argument must be a dict"
            ));

        Dict *dict = dynamic_cast<Dict *>(args[0]);

        dict->elements.emplace_back(args[1], args[2]);

        return *result.success(dict);
    }
};
