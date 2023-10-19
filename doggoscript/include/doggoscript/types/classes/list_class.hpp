#pragma once

#include "../class.hpp"

struct Instance; // From doggoscript/types/instance.hpp

struct ListClass : public BuiltInClass {
    std::vector<Object*> elements;

    static Instance* new_instance(std::vector<Object*> initial_value);

    explicit ListClass(std::vector<Object*> initial_value);

    std::optional<std::string> to_string() override {
        std::string s = "[";
        for(int i = 0; i < this->elements.size(); i++) {
            s += this->elements[i]->str();

            if(i != this->elements.size() - 1) { s += ", "; }
        }
        s += "]";

        return s;
    }
};