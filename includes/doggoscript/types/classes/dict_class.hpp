#pragma once

#include "../class.hpp"

struct Instance; // From doggoscript/types/instance.hpp

struct DictClass : public BuiltInClass {
    std::vector<std::tuple<Object *, Object *>> elements;

    static Instance *new_instance(std::vector<std::tuple<Object *, Object *>> initial_value);

    explicit DictClass(std::vector<std::tuple<Object *, Object *>> initial_value);

    std::optional<std::string> to_string() override {
        std::string s = "{";

        for (size_t i = 0; i < this->elements.size(); i++) {
            auto [key, value] = this->elements[i];
            s += key->str() + " -> " + value->str();

            if (i != this->elements.size() - 1) {
                s += ", ";
            }
        }

        s += "}";

        return s;
    }
};