#pragma once

#include <fstream>
#include "../function.hpp"
#include "../classes/string_class.hpp"

struct LoadFileFunction : public BuiltInFunction {
    LoadFileFunction() : BuiltInFunction("load_file", {"file_name"}, LoadFileFunction::call) {}

    static RuntimeResult call(std::vector<Object *> args) {
        RuntimeResult result;

        BaseClass *file_name_cls = dynamic_cast<Instance*>(args[0])->cls;

        if(file_name_cls->cls_type != BuiltInType::String)
            return *result.failure(ArgumentError(
                *args[0]->start_pos, *args[0]->end_pos,
                "Expected string for file name."
            ));

        StringClass* file_name = static_cast<StringClass*>(file_name_cls);

        std::ifstream file(file_name->value);

        if (!file) {
            return *result.failure(FileNotFoundError(
                *args[0]->start_pos, *args[0]->end_pos,
                std::format("File '{}'", file_name->value)
            ));
        }

        std::string src((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        return *result.success(StringClass::new_instance(src));
    }
};
