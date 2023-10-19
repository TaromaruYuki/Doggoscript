#include <doggoscript/doggoscript.hpp>

DoggoscriptResult run(std::string src, SymbolTable* symbol_table) {
    Lexer       lexer("stdin", src);
    LexerResult lex_result = lexer.tokenize();

    if(lex_result.error.has_value()) {
        return { .error = lex_result.error.value() };
    }

    if(lex_result.tokens.has_value() && lex_result.tokens.value().size() == 1 &&
       lex_result.tokens.value()[0].type == TokenType::FileEnd)
        return {};

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    if(parse_result.error.has_value()) {
        return { .error = parse_result.error.value() };
    }

    Interpreter interpreter;
    Context     context("<prog>");
    context.symbol_table = symbol_table;
    RuntimeResult result = interpreter.visit(parse_result.node, context);

    if(result.error.has_value()) { return { .error = result.error.value() }; }

    if(result.value.has_value() && result.value.value() != nullptr)
        return { .result = result.value.value() };

    return {};
}

void populate_symbol_table(SymbolTable* symbol_table) {
    symbol_table->set("print", new PrintFunction());
    symbol_table->set("input", new InputFunction());
    symbol_table->set("load_file", new LoadFileFunction());
    symbol_table->set("save_file", new SaveFileFunction());
    symbol_table->set("str", new StrFunction());
}
