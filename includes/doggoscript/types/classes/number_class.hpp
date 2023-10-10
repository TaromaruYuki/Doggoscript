#pragma once

#include "boolean_class.hpp"
#include "../class.hpp"
#include "../instance.hpp"
#include "../../error.hpp"

struct Instance; // From doggoscript/types/instance.hpp

struct NumberClass : public BuiltInClass {
    double value;

    static Instance *new_instance(double initial_value);

    explicit NumberClass(double initial_value);

    std::optional<std::string> to_string() override {
        return std::format("{}", this->value);
    }

    bool is_true() override {
        return this->value != 0;
    }

    ObjectResult operator+(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Number) {
                return {NumberClass::new_instance(this->value + dynamic_cast<NumberClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::operator+(other);
    }

    ObjectResult operator-(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Number) {
                return {NumberClass::new_instance(this->value - dynamic_cast<NumberClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::operator-(other);
    }

    ObjectResult operator*(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Number) {
                return {NumberClass::new_instance(this->value * dynamic_cast<NumberClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::operator*(other);
    }

    ObjectResult operator/(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Number) {
                if(dynamic_cast<NumberClass*>(instance->cls)->value == 0)
                    return {std::nullopt, ArithmeticError(this->start_pos.value(), this->end_pos.value(), "Division by zero")};

                return {NumberClass::new_instance(this->value / dynamic_cast<NumberClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::operator/(other);
    }

    ObjectResult logical_and(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Number) {
                return {BooleanClass::new_instance(this->value && dynamic_cast<NumberClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::logical_and(other);
    }

    ObjectResult logical_or(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Number) {
                return {BooleanClass::new_instance(this->value || dynamic_cast<NumberClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::logical_or(other);
    }

    ObjectResult logical_not() override {
        return {BooleanClass::new_instance(!this->value), std::nullopt};
    }

    ObjectResult operator<(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Number) {
                return {BooleanClass::new_instance(this->value < dynamic_cast<NumberClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::operator<(other);
    }

    ObjectResult operator<=(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Number) {
                return {BooleanClass::new_instance(this->value <= dynamic_cast<NumberClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::operator<=(other);
    }

    ObjectResult operator>(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Number) {
                return {BooleanClass::new_instance(this->value > dynamic_cast<NumberClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::operator>(other);
    }

    ObjectResult operator>=(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Number) {
                return {BooleanClass::new_instance(this->value >= dynamic_cast<NumberClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::operator>=(other);
    }

    ObjectResult operator==(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Number) {
                return {BooleanClass::new_instance(this->value == dynamic_cast<NumberClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::operator==(other);
    }

    ObjectResult operator!=(Object &other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::Number) {
                return {BooleanClass::new_instance(this->value != dynamic_cast<NumberClass*>(instance->cls)->value), std::nullopt};
            }
        }

        return Object::operator!=(other);
    }
};