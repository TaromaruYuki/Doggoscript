#pragma once

#include <fstream>
#include "../function.hpp"

struct SaveFileFunction : public BuiltInFunction {
    SaveFileFunction() : BuiltInFunction("save_file", {"file_name", "content"}, SaveFileFunction::call) {}

    static RuntimeResult call(std::vector<Object *> args) {
        RuntimeResult result;

        if (args[0]->type != ObjectType::String)
            return *result.failure(ArgumentError(
                    *args[0]->start_pos, *args[0]->end_pos,
                    "First argument must be a string"
            ));

        auto *file_name = dynamic_cast<String *>(args[0]);

        std::ofstream file(file_name->value);

        file << args[1]->print_friendly();

        if (!file) {
            return *result.failure(BaseError(
                    *args[0]->start_pos, *args[0]->end_pos,
                    "File Error",
                    std::format("File '{}' has threw a error. File may be invalid.", file_name->value)
            ));
        }

        file.close();

        return *result.success(nullptr);
    }
};
