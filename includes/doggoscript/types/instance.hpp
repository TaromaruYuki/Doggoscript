#pragma once

#include <iomanip>
#include "object.hpp"
#include "class.hpp"

struct Instance : public Object {
    BaseClass* cls;
    SymbolTable* symbol_table;

    explicit Instance(BaseClass* cls) : cls(cls) {
        this->type = ObjectType::Instance;
        this->symbol_table = new SymbolTable(cls->symbol_table);
        this->symbol_table->set("$this", this);

        for(auto& [key, value] : cls->symbol_table->symbols) {
            if(value->type == ObjectType::Function) {
                this->symbol_table->set_local(key, value);
            }
        }
    }

    std::string str() override {
        std::stringstream str;
        str << "<" << this->cls->name << " at address 0x" << std::hex << std::setw(sizeof(size_t)) << std::setfill('0') << this << ">";

        return str.str();
    }

    [[nodiscard]] bool item_exists(std::string item) const {
        return this->cls->symbol_table->exists(item);
    }

    RuntimeResult construct(std::vector<Object*> args) {
        RuntimeResult result;

        auto* context = new Context("<constructor>");
        context->symbol_table = this->symbol_table;
        context->parent = this->context;

        if(this->cls->symbol_table->exists("__constructor")) {
            auto* func_o = static_cast<BaseFunction*>(this->cls->symbol_table->get("__constructor"));
            func_o->set_context(context);

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
