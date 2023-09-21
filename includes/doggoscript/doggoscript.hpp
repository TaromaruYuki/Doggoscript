#pragma once

#include <optional>
#include <string>
#include <sstream>
#include "context.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "error.hpp"
#include "types/types.hpp"

struct DoggoscriptResult {
    std::optional<Object *> result;
    std::optional<BaseError> error;
};

DoggoscriptResult run(std::string src, SymbolTable *symbol_table);

void populate_symbol_table(SymbolTable *symbol_table);
