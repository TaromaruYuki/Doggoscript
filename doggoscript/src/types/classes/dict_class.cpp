#include <doggoscript/types/classes/dict_class.hpp>

#include <doggoscript/types/instance.hpp>

Instance *DictClass::new_instance(std::vector<std::tuple<Object *, Object *>> initial_value) {
    auto *str_cls = new DictClass(initial_value);
    auto *instance = new Instance(str_cls);

    return instance;
}

DictClass::DictClass(std::vector<std::tuple<Object *, Object *>> initial_value) : BuiltInClass("Dict") {
    this->elements = initial_value;
    this->cls_type = BuiltInType::Dict;

    this->symbol_table->set("get", new BuiltInFunction("get", {"key"}, [this](std::vector<Object *> args) -> RuntimeResult {
        RuntimeResult result;

        for (auto &element: this->elements) {
            Object *key = std::get<0>(element);

            // FIXME: This feels super hacky using str()
            if (key->str() == args[0]->str()) {
                Object *value = std::get<1>(element);
                return *result.success(value);
            }
        }

        return *result.failure(ArgumentError(
                *args[0]->start_pos, *args[0]->end_pos,
                "Key not found"
        ));
    }));

    this->symbol_table->set("set", new BuiltInFunction("set", {"key", "value"}, [this](std::vector<Object *> args) -> RuntimeResult {
        RuntimeResult result;

        this->elements.emplace_back(args[0], args[1]);

        return *result.success(new Instance(this));
    }));
}
