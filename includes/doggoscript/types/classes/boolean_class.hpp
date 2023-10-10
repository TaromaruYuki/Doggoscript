#pragma once

#include "../class.hpp"
#include "../instance.hpp"

struct BooleanClass : public BuiltInClass {
    bool value;

    static Instance *new_instance(bool initial_value);

    explicit BooleanClass(bool initial_value);

    std::optional<std::string> to_string() override {
        return value ? "true" : "false";
    }

    bool is_true() override {
        return this->value;
    }

    ObjectResult logical_and(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Boolean) {
                return {BooleanClass::new_instance(this->value && dynamic_cast<BooleanClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::logical_and(other);
    }

    ObjectResult logical_or(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Boolean) {
                return {BooleanClass::new_instance(this->value || dynamic_cast<BooleanClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::logical_or(other);
    }

    ObjectResult logical_not() override {
        return {BooleanClass::new_instance(!this->value), std::nullopt};
    }
};