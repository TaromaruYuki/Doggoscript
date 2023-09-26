#pragma once

#include <fstream>
#include "../function.hpp"

struct LoadFileFunction : public BuiltInFunction {
    LoadFileFunction() : BuiltInFunction("load_file", {"file_name"}, LoadFileFunction::call) {}

    static RuntimeResult call(std::vector<Object *> args) {
        RuntimeResult result;

        if (args[0]->type != ObjectType::String)
            return *result.failure(ArgumentError(
                    *args[0]->start_pos, *args[0]->end_pos,
                    "First argument must be a string"
            ));

        String *file_name = dynamic_cast<String *>(args[0]);

        std::ifstream file(file_name->value);

        if (!file) {
            return *result.failure(FileNotFoundError(
                *args[0]->start_pos, *args[0]->end_pos,
                std::format("File '{}'", file_name->value)
            ));
        }

        std::string src((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        return *result.success(new String(src));
    }
};
