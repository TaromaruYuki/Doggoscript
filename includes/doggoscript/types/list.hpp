#pragma once

#include <tuple>
#include <vector>
#include "object.hpp"

struct List;

typedef std::tuple<std::optional<List>, std::optional<BaseError>> ListResult;

struct List : public Object {
    std::vector<Object *> elements;

    List() { this->type = ObjectType::List; }

    explicit List(std::vector<Object *> elements) : elements(elements) { this->type = ObjectType::List; }

    std::string str() {
        std::string s = "[";
        for (int i = 0; i < this->elements.size(); i++) {
            s += this->elements[i]->str();

            if (i != this->elements.size() - 1) {
                s += ", ";
            }
        }
        s += "]";

        return s;
    }

    bool is_true() override {
        return !this->elements.empty();
    }
};