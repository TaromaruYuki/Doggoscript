#pragma once

#include "object.hpp"
#include "function.hpp"
#include "../context.hpp"

struct BaseClass : public Object {
    std::string name;
    SymbolTable *symbol_table;

    explicit BaseClass(std::string name) : name(std::move(name)) {
        this->type = ObjectType::Class;
        this->symbol_table = new SymbolTable();
    }

    std::string str() override {
        return "<class " + this->name + ">";
    }
};

struct Class : public BaseClass {
    std::vector<Object*> methods;

    Class(std::string name, std::vector<Object*> methods) : BaseClass(std::move(name)), methods(methods) {
        for(Object* method : methods) {
            if(method->type == ObjectType::Function) {
                auto* func = static_cast<BaseFunction*>(method);
                this->symbol_table->set(func->name, func);
            }
        }
    }
};
