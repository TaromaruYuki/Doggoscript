#pragma once

#include "object.hpp"

#include <tuple>
#include <vector>

struct Statements : public Object {
    std::vector<Object*> elements;

    explicit Statements(std::vector<Object*> elements) : elements(elements) {
        this->type = ObjectType::Statements;
    }

    std::string str() {
        std::string s;
        for(int i = 0; i < this->elements.size(); i++) {
            s += this->elements[i]->str();

            if(i != this->elements.size() - 1) { s += "\n"; }
        }

        return s;
    }
};