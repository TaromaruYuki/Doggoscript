#pragma once

#include "../class.hpp"
#include "../instance.hpp"

struct StringClass : public BuiltInClass {
    std::string value;

    static Instance* new_instance(std::string initial_value);

    explicit StringClass(std::string initial_value);

    std::optional<std::string> to_string() override {
        return "\"" + this->value + "\"";
    }

    std::string print_friendly() override { return this->value; }

    ObjectResult operator+(Object& other) override {
        if(other.type == ObjectType::Instance) {
            auto* instance = dynamic_cast<Instance*>(&other);
            if(instance->cls->cls_type == BuiltInType::String) {
                return { this->new_instance(
                             this->value +
                             dynamic_cast<StringClass*>(instance->cls)->value),
                         std::nullopt };
            }
        }

        return Object::operator+(other);
    }
};