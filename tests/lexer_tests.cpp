#include <doggoscript/lexer.hpp>
#include <gtest/gtest.h>

/*
 * Characters with functions
 */

TEST(Lexer, Integer) {
    Lexer       lexer("test", "123");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Int);
    ASSERT_EQ(result.tokens.value()[0].value, "123");
}

TEST(Lexer, Float) {
    Lexer       lexer("test", "123.456");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Float);
    ASSERT_EQ(result.tokens.value()[0].value, "123.456");
}

TEST(Lexer, InvalidFloat) {
    Lexer       lexer("test", "123.45.6");
    LexerResult result = lexer.tokenize();

    ASSERT_TRUE(result.error.has_value());
    ASSERT_EQ(result.error.value().error_name, "Illegal Character");
}

TEST(Lexer, CreateRangeWithTwoNumbers) {
    Lexer       lexer("test", "1..10");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 4);

    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Int);
    ASSERT_EQ(result.tokens.value()[0].value, "1");

    ASSERT_EQ(result.tokens.value()[1].type, TokenType::Through);

    ASSERT_EQ(result.tokens.value()[2].type, TokenType::Int);
    ASSERT_EQ(result.tokens.value()[2].value, "10");
}

TEST(Lexer, CreateRangeWithOneNumber) {
    Lexer       lexer("test", "1..");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 3);

    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Int);
    ASSERT_EQ(result.tokens.value()[0].value, "1");

    ASSERT_EQ(result.tokens.value()[1].type, TokenType::Through);
}

TEST(Lexer, CreateRangeNoNumbers) {
    Lexer       lexer("test", "..");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Through);
}

TEST(Lexer, CreateMultiply) {
    Lexer       lexer("test", "*");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Multiply);
}

TEST(Lexer, CreatePower) {
    Lexer       lexer("test", "**");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Power);
}

TEST(Lexer, CreateString) {
    Lexer       lexer("test", "\"Hello, world!\"");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::String);
    ASSERT_EQ(result.tokens.value()[0].value, "Hello, world!");
}

TEST(Lexer, CreateStringWithEscapedCharacters) {
    Lexer       lexer("test", "\"Hello, world!\\n\\t\\r\"");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::String);
    ASSERT_EQ(result.tokens.value()[0].value, "Hello, world!\n\t\r");
    ASSERT_NE(result.tokens.value()[0].value, "Hello, world!\\n\\t\\r");
}

TEST(Lexer, CreateStringWithEscapedEscapeCharacters) {
    Lexer       lexer("test", "\"Hello, world!\\\\n\\\\t\\\\r\"");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::String);
    ASSERT_EQ(result.tokens.value()[0].value, "Hello, world!\\n\\t\\r");
    ASSERT_NE(result.tokens.value()[0].value, "Hello, world!\n\t\r");
}

TEST(Lexer, CreateStringWithNoEnd) {
    Lexer       lexer("test", "\"Hello!");
    LexerResult result = lexer.tokenize();

    ASSERT_TRUE(result.error.has_value());
    ASSERT_EQ(result.error.value().error_name, "Expected Character");
}

TEST(Lexer, CreateIdentifier) {
    Lexer       lexer("test", "hello");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Identifier);
    ASSERT_EQ(result.tokens.value()[0].value, "hello");
}

TEST(Lexer, CreateKeyword) {
    Lexer       lexer("test", "class");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Keyword);
    ASSERT_EQ(result.tokens.value()[0].value, "class");
    ASSERT_TRUE(result.tokens.value()[0].is_keyword("class"));
}

TEST(Lexer, CreateVariable) {
    Lexer       lexer("test", "$hello");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Variable);
    ASSERT_EQ(result.tokens.value()[0].value, "$hello");
}

TEST(Lexer, CreateVariableWithKeywordName) {
    Lexer       lexer("test", "$class");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Variable);
    ASSERT_EQ(result.tokens.value()[0].value, "$class");
    ASSERT_FALSE(result.tokens.value()[0].is_keyword("class"));
}

TEST(Lexer, CreateNot) {
    Lexer       lexer("test", "!");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::LogicalNot);
}

TEST(Lexer, CreateNotEquals) {
    Lexer       lexer("test", "!=");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::NotEquals);
}

TEST(Lexer, CreateGreaterThan) {
    Lexer       lexer("test", ">");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::GreaterThan);
}

TEST(Lexer, CreateGreaterThanEquals) {
    Lexer       lexer("test", ">=");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::GreaterThanEquals);
}

TEST(Lexer, CreateLessThan) {
    Lexer       lexer("test", "<");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::LessThan);
}

TEST(Lexer, CreateLessThanEquals) {
    Lexer       lexer("test", "<=");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::LessThanEquals);
}

TEST(Lexer, CreateBitwiseAndError) {
    Lexer       lexer("test", "&");
    LexerResult result = lexer.tokenize();

    ASSERT_TRUE(result.error.has_value());
    ASSERT_EQ(result.error.value().error_name, "Expected Character");
}

TEST(Lexer, CreateLogicalAnd) {
    Lexer       lexer("test", "&&");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::LogicalAnd);
}

TEST(Lexer, CreateBitwiseOrError) {
    Lexer       lexer("test", "|");
    LexerResult result = lexer.tokenize();

    ASSERT_TRUE(result.error.has_value());
    ASSERT_EQ(result.error.value().error_name, "Expected Character");
}

TEST(Lexer, CreateLogicalOr) {
    Lexer       lexer("test", "||");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::LogicalOr);
}

TEST(Lexer, CreateMinus) {
    Lexer       lexer("test", "-");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Minus);
}

TEST(Lexer, CreateArrow) {
    Lexer       lexer("test", "->");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Arrow);
}

TEST(Lexer, CreateDiv) {
    Lexer       lexer("test", "/");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 2);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Divide);
}

TEST(Lexer, CreateComment) {
    Lexer       lexer("test", "// Hello!");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value().size(), 1);
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::FileEnd);
}

/*
 * Single Characters
 */

TEST(Lexer, CreatePlus) {
    Lexer       lexer("test", "+");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Plus);
}

TEST(Lexer, CreateLParen) {
    Lexer       lexer("test", "(");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::LParen);
}

TEST(Lexer, CreateRParen) {
    Lexer       lexer("test", ")");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::RParen);
}

TEST(Lexer, CreateLSquare) {
    Lexer       lexer("test", "[");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::LSquare);
}

TEST(Lexer, CreateRSquare) {
    Lexer       lexer("test", "]");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::RSquare);
}

TEST(Lexer, CreateLCurly) {
    Lexer       lexer("test", "{");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::LCurly);
}

TEST(Lexer, CreateRCurly) {
    Lexer       lexer("test", "}");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::RCurly);
}

TEST(Lexer, CreateEqualsTo) {
    Lexer       lexer("test", "=");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::EqualsTo);
}

TEST(Lexer, CreateComma) {
    Lexer       lexer("test", ",");
    LexerResult result = lexer.tokenize();

    ASSERT_FALSE(result.error.has_value())
        << "Error: " << result.error.value().str();
    ASSERT_TRUE(result.tokens.has_value());
    ASSERT_EQ(result.tokens.value()[0].type, TokenType::Comma);
}