#pragma once

#include <optional>
#include "../position.hpp"
#include "../context.hpp"
#include "../error.hpp"

enum class ObjectType {
    Boolean,
    Function,
    List,
    Number,
    Object,
    Statements,
    String,
};

struct Object {
    ObjectType type;
    std::optional<Position> start_pos;
    std::optional<Position> end_pos;
    Context* context;

    void set_pos(Position start_pos, Position end_pos) {
        this->start_pos = start_pos;
        this->end_pos = end_pos;
    }

    void set_context(Context* context) {
        this->context = context;
    }

    virtual std::string str() {
        return "<object>";
    }

    virtual std::string print_friendly() {
        return this->str();
    }

    virtual bool is_true() {
        return true;
    }
};