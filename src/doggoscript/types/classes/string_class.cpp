#include "../../../../includes/doggoscript/types/classes/string_class.hpp"

#include <algorithm>
#include "../../../../includes/doggoscript/types/function.hpp"
#include "../../../../includes/doggoscript/types/instance.hpp"
#include "../../../../includes/doggoscript/types/classes/number_class.hpp"

Instance *StringClass::new_instance(std::string initial_value) {
    auto* str_cls = new StringClass(initial_value);
    auto* instance = new Instance(str_cls);

    return instance;
}

StringClass::StringClass(std::string initial_value) : BuiltInClass("String") {
    this->value = initial_value;
    this->cls_type = BuiltInType::String;

    this->symbol_table->set("length", new BuiltInFunction("length", {}, [this](std::vector<Object*> args) -> RuntimeResult {
        return *RuntimeResult().success(NumberClass::new_instance(this->value.length()));
    }));

    this->symbol_table->set("lower", new BuiltInFunction("lower", {}, [this](std::vector<Object*> args) -> RuntimeResult {
        std::string lower = this->value;
        std::transform(lower.begin(), lower.end(), lower.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        return *RuntimeResult().success(this->new_instance(lower));
    }));

    this->symbol_table->set("upper", new BuiltInFunction("upper", {}, [this](std::vector<Object*> args) -> RuntimeResult {
        std::string upper = this->value;
        std::transform(upper.begin(), upper.end(), upper.begin(),
                       [](unsigned char c){ return std::toupper(c); });

        return *RuntimeResult().success(this->new_instance(upper));
    }));

    this->symbol_table->set("replace", new BuiltInFunction("replace", {"old", "new"}, [this](std::vector<Object*> args) -> RuntimeResult {
        RuntimeResult result;

        BaseClass* old_cls = static_cast<Instance*>(args[0])->cls;
        BaseClass* new_cls = static_cast<Instance*>(args[1])->cls;

        if(old_cls->cls_type != BuiltInType::String || new_cls->cls_type != BuiltInType::String)
            return *result.failure(ArgumentError(
                    *this->start_pos, *this->end_pos,
                    "Invalid arguments for replace function."
            ));

        std::string old = static_cast<StringClass*>(old_cls)->value;
        std::string new_ = static_cast<StringClass*>(new_cls)->value;

        std::string replaced = this->value;

        size_t start_pos = 0;
        while((start_pos = replaced.find(old, start_pos)) != std::string::npos) {
            replaced.replace(start_pos, old.length(), new_);
            start_pos += new_.length();
        }

        return *result.success(this->new_instance(replaced));
    }));

    this->symbol_table->set("at", new BuiltInFunction("at", {"index"}, [this](std::vector<Object*> args) -> RuntimeResult {
        RuntimeResult result;

        BaseClass* index_cls = static_cast<Instance*>(args[0])->cls;

        if(index_cls->cls_type != BuiltInType::Number)
            return *result.failure(ArgumentError(
                    *this->start_pos, *this->end_pos,
                    "Invalid arguments for at function."
            ));

        int index = static_cast<NumberClass*>(index_cls)->value;

        if(index < 0 || index >= this->value.length())
            return *result.failure(ArgumentError(
                    *this->start_pos, *this->end_pos,
                    "Index out of range."
            ));

        return *result.success(this->new_instance(std::string(1, this->value[index])));
    }));
}
