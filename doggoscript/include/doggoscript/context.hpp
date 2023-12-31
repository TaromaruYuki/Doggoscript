#pragma once

#include "position.hpp"

#include <map>
#include <optional>
#include <string>

struct Object;

struct SymbolTable {
    std::map<std::string, Object*> symbols;
    std::optional<SymbolTable*>    parent;

    SymbolTable() { this->parent = std::nullopt; }

    explicit SymbolTable(SymbolTable* parent) { this->parent = parent; }

    Object* get(const std::string& name) {
        if(this->symbols.contains(name)) return this->symbols[name];

        if(this->parent.has_value()) return this->parent.value()->get(name);

        return nullptr;
    }

    Object* get_local(const std::string& name) {
        if(this->symbols.contains(name)) return this->symbols[name];

        return nullptr;
    }

    void set(std::string name, Object* value) {
        if(this->parent.has_value()) {
            if(this->parent.value()->get(name) != nullptr) {
                this->parent.value()->set(name, value);
                return;
            }
        }

        this->symbols[name] = value;
    }

    void set_local(std::string name, Object* value) {
        this->symbols[name] = value;
    }

    void remove(const std::string& name) { this->symbols.erase(name); }

    bool exists(const std::string& name) {
        if(this->symbols.contains(name)) return true;

        if(this->parent.has_value()) return this->parent.value()->exists(name);

        return false;
    }

    bool exists_local(const std::string& name) {
        return this->symbols.contains(name);
    }
};

struct Context {
    std::string  display_name;
    Context*     parent       = nullptr;
    Position*    entry_pos    = nullptr;
    SymbolTable* symbol_table = nullptr;

    Context(std::string display_name) : display_name(display_name) {}
};
