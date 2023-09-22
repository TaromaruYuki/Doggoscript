#pragma once

#include <tuple>
#include <utility>
#include <vector>
#include "object.hpp"

struct Dict;

typedef std::tuple<std::optional<Dict>, std::optional<BaseError>> DictResult;

struct Dict : public Object {
    std::vector<std::tuple<Object *, Object *>> elements;

    Dict() { this->type = ObjectType::List; }

    explicit Dict(std::vector<std::tuple<Object *, Object *>> elements) : elements(
            std::move(elements)) { this->type = ObjectType::Dict; }

    std::string str() {
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

    bool is_true() override {
        return !this->elements.empty();
    }
};