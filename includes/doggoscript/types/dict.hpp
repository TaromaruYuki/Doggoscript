#pragma once

#include <tuple>
#include <vector>
#include <map>
#include "object.hpp"

struct Dict;

typedef std::tuple<std::optional<Dict>, std::optional<BaseError>> DictResult;

struct Dict : public Object {
    std::map<Object *, Object *> elements;

    Dict() { this->type = ObjectType::List; }

    explicit Dict(std::map<Object *, Object *> elements) : elements(elements) { this->type = ObjectType::Dict; }

    std::string str() {
        std::string s = "{";

        for (auto const &[key, value]: this->elements) {
            s += key->str() + ": " + value->str() + ", ";
        }

        s += "}";

        return s;
    }

    bool is_true() override {
        return !this->elements.empty();
    }
};