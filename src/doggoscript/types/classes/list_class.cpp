#include "../../../../includes/doggoscript/types/classes/list_class.hpp"

#include <format>
#include "../../../../includes/doggoscript/types/instance.hpp"
#include "../../../../includes/doggoscript/types/classes/string_class.hpp"
#include "../../../../includes/doggoscript/types/classes/number_class.hpp"

Instance *ListClass::new_instance(std::vector<Object*> initial_value) {
    auto *str_cls = new ListClass(initial_value);
    auto *instance = new Instance(str_cls);

    return instance;
}

ListClass::ListClass(std::vector<Object*> initial_value) : BuiltInClass("List") {
    this->elements = initial_value;
    this->cls_type = BuiltInType::List;

    this->symbol_table->set("length", new BuiltInFunction("length", {}, [this](std::vector<Object*> args) -> RuntimeResult {
        return *RuntimeResult().success(NumberClass::new_instance(this->elements.size()));
    }));

    this->symbol_table->set("append", new BuiltInFunction("append", { "element" }, [this](std::vector<Object*> args) -> RuntimeResult {
        this->elements.push_back(args[0]);

        return *RuntimeResult().success(NumberClass::new_instance(this->elements.size()));
    }));

    this->symbol_table->set("extend", new BuiltInFunction("extend", { "list" }, [this](std::vector<Object*> args) -> RuntimeResult {
        auto* list_cls = static_cast<Instance*>(args[0])->cls;

        if(list_cls->cls_type != BuiltInType::List)
            return *RuntimeResult().failure(ArgumentError(
                *this->start_pos, *this->end_pos,
                "Invalid arguments for extend function."
            ));

        auto* list = static_cast<ListClass*>(list_cls);

        for (auto* element : list->elements) {
            this->elements.push_back(element);
        }

        return *RuntimeResult().success(new Instance(this));
    }));

    this->symbol_table->set("pop", new BuiltInFunction("pop", { "index" }, [this](std::vector<Object*> args) -> RuntimeResult {
        auto* index_cls = static_cast<Instance*>(args[0])->cls;

        if (index_cls->cls_type != BuiltInType::Number) {
            return *RuntimeResult().failure(ArgumentError(
                *this->start_pos, *this->end_pos,
                "Argument must be a number"
            ));
        }

        auto* index = static_cast<NumberClass*>(index_cls);

        if (index->value < 0 || index->value >= this->elements.size()) {
            return *RuntimeResult().failure(ArgumentError(
                *this->start_pos, *this->end_pos,
                "Index out of range"
            ));
        }

        auto* element = this->elements[index->value];
        this->elements.erase(this->elements.begin() + index->value);

        return *RuntimeResult().success(element);
    }));

    this->symbol_table->set("at", new BuiltInFunction("at", { "index" }, [this](std::vector<Object*> args) -> RuntimeResult {
        auto* index_cls = static_cast<Instance*>(args[0])->cls;

        if (index_cls->cls_type != BuiltInType::Number) {
            return *RuntimeResult().failure(ArgumentError(
                    *this->start_pos, *this->end_pos,
                    "Argument must be a number"
            ));
        }

        auto* index = static_cast<NumberClass*>(index_cls);

        if (index->value < 0 || index->value >= this->elements.size()) {
            return *RuntimeResult().failure(ArgumentError(
                    *this->start_pos, *this->end_pos,
                    "Index out of range"
            ));
        }

        return *RuntimeResult().success(this->elements[index->value]);
    }));

    this->symbol_table->set("set", new BuiltInFunction("set", {"index", "element"}, [this](std::vector<Object*> args) -> RuntimeResult {
        auto* index_o = static_cast<Instance*>(args[0])->cls;

        if (index_o->cls_type != BuiltInType::Number) {
            return *RuntimeResult().failure(ArgumentError(
                    *this->start_pos, *this->end_pos,
                    "Argument must be a number"
            ));
        }

        auto* index = static_cast<NumberClass*>(index_o);

        if (index->value < 0 || index->value >= this->elements.size()) {
            return *RuntimeResult().failure(ArgumentError(
                    *this->start_pos, *this->end_pos,
                    "Index out of range"
            ));
        }

        this->elements[index->value] = args[1];

        return *RuntimeResult().success(args[1]);
    }));
}
