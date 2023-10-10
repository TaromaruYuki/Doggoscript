#pragma once

#include <fstream>
#include "../function.hpp"

struct SaveFileFunction : public BuiltInFunction {
    SaveFileFunction() : BuiltInFunction("save_file", {"file_name", "content"}, SaveFileFunction::call) {}

    static RuntimeResult call(std::vector<Object *> args) {
        RuntimeResult result;

        BaseClass *file_name_cls = dynamic_cast<Instance*>(args[0])->cls;

        if(file_name_cls->cls_type != BuiltInType::String)
            return *result.failure(ArgumentError(
                    *args[0]->start_pos, *args[0]->end_pos,
                    "Expected string for file name."
            ));

        StringClass* file_name = static_cast<StringClass*>(file_name_cls);

        std::ofstream file(file_name->value);

        file << args[1]->print_friendly();

        if (!file) {
            return *result.failure(BaseError(
                    *args[0]->start_pos, *args[0]->end_pos,
                    "File Error",
                    "File '" + file_name->value + "' has threw a error. File may be invalid."
            ));
        }

        file.close();

        return *result.success(nullptr);
    }
};
