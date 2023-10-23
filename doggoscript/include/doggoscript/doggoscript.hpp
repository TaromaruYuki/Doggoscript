#pragma once

#include "context.hpp"
#include "error.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "types/types.hpp"

#include <optional>
#include <sstream>
#include <string>

struct DoggoscriptResult {
    std::optional<Object*>   result;
    std::optional<BaseError> error;
};

DoggoscriptResult run(std::string src, SymbolTable* symbol_table);

void populate_symbol_table(SymbolTable* symbol_table);
