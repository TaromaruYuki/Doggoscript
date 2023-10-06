#pragma once

#include "object.hpp"
#include "function.hpp"
#include "../context.hpp"

enum class BuiltInType {
    UserCreated,
    String,
    Number,
    Boolean,
    List,
    Dict
};

struct BaseClass : public Object {
    std::string name;
    SymbolTable *symbol_table;
    BuiltInType cls_type;

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
        this->cls_type = BuiltInType::UserCreated;

        for(Object* method : methods) {
            if(method->type == ObjectType::Function) {
                auto* func = static_cast<BaseFunction*>(method);
                this->symbol_table->set(func->name, func);
            }
        }
    }
};

struct BuiltInClass : public BaseClass {
    BuiltInClass(std::string name) : BaseClass(std::move(name)) { }

    virtual std::optional<std::string> to_string() { return std::nullopt; }
};
