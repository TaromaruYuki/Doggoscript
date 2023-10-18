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
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            BuiltInClass* cls = static_cast<BuiltInClass*>(this->cls);
            if(std::optional<std::string> str = cls->to_string()) {
                return str.value();
            }
        } else {
            if(this->item_exists("__string")) {
                auto* func_o = static_cast<BaseFunction*>(this->symbol_table->get("__string"));
                func_o->set_context(this->context);

                if(func_o->func_type == FunctionType::UserDefined) {
                    auto* func = static_cast<Function*>(func_o);
                    auto result = func->operator()({});

                    if(result.error.has_value())
                        return "<error>";

                    return result.value.value()->print_friendly();
                }
            }
        }

        std::stringstream str;
        str << "<" << this->cls->name << " at address 0x" << std::hex << std::setw(sizeof(size_t)) << std::setfill('0') << this << ">";

        return str.str();
    }

    std::string print_friendly() override {
        return this->cls->print_friendly();
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

    bool is_true() override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->is_true();
        }

        return Object::is_true();
    }

    ObjectResult operator+(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->operator+(other);
        }

        return Object::operator+(other);
    }

    ObjectResult operator-(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->operator-(other);
        }

        return Object::operator-(other);
    }

    ObjectResult operator*(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->operator*(other);
        }

        return Object::operator*(other);
    }

    ObjectResult operator/(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->operator/(other);
        }

        return Object::operator/(other);
    }

    ObjectResult logical_and(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->logical_and(other);
        }

        return Object::logical_and(other);
    }

    ObjectResult logical_or(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->logical_or(other);
        }

        return Object::logical_or(other);
    }

    ObjectResult logical_not() override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->logical_not();
        }

        return Object::logical_not();
    }

    ObjectResult power_by(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->power_by(other);
        }

        return Object::power_by(other);
    }

    ObjectResult operator<(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->operator<(other);
        }

        return Object::operator<(other);
    }

    ObjectResult operator<=(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->operator<=(other);
        }

        return Object::operator<=(other);
    }

    ObjectResult operator>(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->operator>(other);
        }

        return Object::operator>(other);
    }

    ObjectResult operator>=(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->operator>=(other);
        }

        return Object::operator>=(other);
    }

    ObjectResult operator==(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->operator==(other);
        }

        return Object::operator==(other);
    }

    ObjectResult operator!=(Object &other) override {
        if(this->cls->cls_type != BuiltInType::UserCreated) {
            return this->cls->operator!=(other);
        }

        return Object::operator!=(other);
    }
};
