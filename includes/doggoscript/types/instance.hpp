#pragma once

#include "object.hpp"
#include "class.hpp"

struct Instance : public Object {
    BaseClass* cls;
    SymbolTable* symbol_table;

    explicit Instance(BaseClass* cls) : cls(cls) {
        this->type = ObjectType::Instance;
        this->symbol_table = new SymbolTable(cls->symbol_table);
    }

    std::string str() override {
        return "<instance of " + this->cls->name + ">";
    }

    [[nodiscard]] bool item_exists(std::string item) const {
        return this->cls->symbol_table->exists(item);
    }

    RuntimeResult construct(std::vector<Object*> args) {
        RuntimeResult result;

        if(this->cls->symbol_table->exists("__constructor")) {
            auto* func_o = static_cast<BaseFunction*>(this->cls->symbol_table->get("__constructor"));
            func_o->set_context(this->context);

            if(func_o->func_type == FunctionType::UserDefined) {
                auto* func = static_cast<Function*>(func_o);
                result = func->operator()(args);
            } else {
                auto* func = static_cast<BuiltInFunction*>(func_o);
                result = func->operator()(args);
            }

            if(result.error.has_value())
                return result;
        }

        return *result.success(this);
    }
};
