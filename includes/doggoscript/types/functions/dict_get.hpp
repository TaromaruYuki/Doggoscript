#pragma once

#include "../function.hpp"

struct DictGetFunction : public BuiltInFunction {
    DictGetFunction() : BuiltInFunction("dict_get", {"dict", "key"}, DictGetFunction::call) {}

    static RuntimeResult call(std::vector<Object *> args) {
        RuntimeResult result;

        if (args[0]->type != ObjectType::Dict)
            return *result.failure(ArgumentError(
                    *args[0]->start_pos, *args[0]->end_pos,
                    "First argument must be a dict"
            ));

        Dict *dict = dynamic_cast<Dict *>(args[0]);

        for (auto &element: dict->elements) {
            Object *key = std::get<0>(element);

            // FIXME: This feels super hacky using str()
            if (key->str() == args[1]->str()) {
                Object *value = std::get<1>(element);
                return *result.success(value);
            }
        }

        return *result.failure(ArgumentError(
                *args[0]->start_pos, *args[0]->end_pos,
                "Key not found"
        ));
    }
};
