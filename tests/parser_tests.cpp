#include <doggoscript/lexer.hpp>
#include <doggoscript/parser.hpp>
#include <gtest/gtest.h>
#include <iostream>

#define DICT_KEY 0
#define DICT_VAL 1
#define IF_COND  0
#define IF_EXPR  1
#define IF_RET   2

TEST(Parser, Integer) {
    Lexer       lexer("test", "123");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statement_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statement_node->statements.size(), 1);

    ASSERT_EQ(statement_node->statements[0]->type, NodeType::NumberNode);
    auto* number_node = static_cast<NumberNode*>(statement_node->statements[0]);
    ASSERT_EQ(number_node->token->type, TokenType::Int);
    ASSERT_EQ(number_node->token->value, "123");
}

TEST(Parser, Float) {
    Lexer       lexer("test", "123.456");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statement_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statement_node->statements.size(), 1);

    ASSERT_EQ(statement_node->statements[0]->type, NodeType::NumberNode);
    auto* number_node = static_cast<NumberNode*>(statement_node->statements[0]);
    ASSERT_EQ(number_node->token->type, TokenType::Float);
    ASSERT_EQ(number_node->token->value, "123.456");
}

TEST(Parser, AddTwoNumbers) {
    Lexer       lexer("test", "123 + 456");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statement_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statement_node->statements.size(), 1);

    ASSERT_EQ(statement_node->statements[0]->type,
              NodeType::BinaryOperationNode);
    auto* binary_operation_node =
        static_cast<BinaryOperationNode*>(statement_node->statements[0]);

    ASSERT_EQ(binary_operation_node->lhs->type, NodeType::NumberNode);
    ASSERT_EQ(binary_operation_node->rhs->type, NodeType::NumberNode);
    ASSERT_EQ(binary_operation_node->token->type, TokenType::Plus);
}

TEST(Parser, SubtractTwoNumbers) {
    Lexer       lexer("test", "123 - 456");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statement_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statement_node->statements.size(), 1);

    ASSERT_EQ(statement_node->statements[0]->type,
              NodeType::BinaryOperationNode);
    auto* binary_operation_node =
        static_cast<BinaryOperationNode*>(statement_node->statements[0]);

    ASSERT_EQ(binary_operation_node->lhs->type, NodeType::NumberNode);
    ASSERT_EQ(binary_operation_node->rhs->type, NodeType::NumberNode);
    ASSERT_EQ(binary_operation_node->token->type, TokenType::Minus);
}

TEST(Parser, MultiplyTwoNumbers) {
    Lexer       lexer("test", "123 * 456");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statement_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statement_node->statements.size(), 1);

    ASSERT_EQ(statement_node->statements[0]->type,
              NodeType::BinaryOperationNode);
    auto* binary_operation_node =
        static_cast<BinaryOperationNode*>(statement_node->statements[0]);

    ASSERT_EQ(binary_operation_node->lhs->type, NodeType::NumberNode);
    ASSERT_EQ(binary_operation_node->rhs->type, NodeType::NumberNode);
    ASSERT_EQ(binary_operation_node->token->type, TokenType::Multiply);
}

TEST(Parser, DIvideTwoNumbers) {
    Lexer       lexer("test", "123 / 456");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statement_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statement_node->statements.size(), 1);

    ASSERT_EQ(statement_node->statements[0]->type,
              NodeType::BinaryOperationNode);
    auto* binary_operation_node =
        static_cast<BinaryOperationNode*>(statement_node->statements[0]);

    ASSERT_EQ(binary_operation_node->lhs->type, NodeType::NumberNode);
    ASSERT_EQ(binary_operation_node->rhs->type, NodeType::NumberNode);
    ASSERT_EQ(binary_operation_node->token->type, TokenType::Divide);
}

TEST(Parser, PowerTwoNumbers) {
    Lexer       lexer("test", "123 ** 2");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statement_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statement_node->statements.size(), 1);

    ASSERT_EQ(statement_node->statements[0]->type,
              NodeType::BinaryOperationNode);
    auto* binary_operation_node =
        static_cast<BinaryOperationNode*>(statement_node->statements[0]);

    ASSERT_EQ(binary_operation_node->lhs->type, NodeType::NumberNode);
    ASSERT_EQ(binary_operation_node->rhs->type, NodeType::NumberNode);
    ASSERT_EQ(binary_operation_node->token->type, TokenType::Power);
}

TEST(Parser, OrderOfOperations) {
    Lexer       lexer("test", "1 + 2 * 3 ** 4");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statement_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statement_node->statements.size(), 1);

    ASSERT_EQ(statement_node->statements[0]->type,
              NodeType::BinaryOperationNode);
    auto* binary_operation_node =
        static_cast<BinaryOperationNode*>(statement_node->statements[0]);

    ASSERT_EQ(binary_operation_node->lhs->type, NodeType::NumberNode);
    ASSERT_EQ(binary_operation_node->rhs->type, NodeType::BinaryOperationNode);
    ASSERT_EQ(binary_operation_node->token->type, TokenType::Plus);

    auto* rhs = static_cast<BinaryOperationNode*>(binary_operation_node->rhs);
    ASSERT_EQ(rhs->lhs->type, NodeType::NumberNode);
    ASSERT_EQ(rhs->rhs->type, NodeType::BinaryOperationNode);
    ASSERT_EQ(rhs->token->type, TokenType::Multiply);

    auto* rhs_rhs = static_cast<BinaryOperationNode*>(rhs->rhs);
    ASSERT_EQ(rhs_rhs->lhs->type, NodeType::NumberNode);
    ASSERT_EQ(rhs_rhs->rhs->type, NodeType::NumberNode);
    ASSERT_EQ(rhs_rhs->token->type, TokenType::Power);
}

TEST(Parser, OrderOfOperationsWithParens) {
    Lexer       lexer("test", "(1 + 2) * 3");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statement_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statement_node->statements.size(), 1);

    ASSERT_EQ(statement_node->statements[0]->type,
              NodeType::BinaryOperationNode);
    auto* binary_operation_node =
        static_cast<BinaryOperationNode*>(statement_node->statements[0]);

    ASSERT_EQ(binary_operation_node->lhs->type, NodeType::BinaryOperationNode);
    ASSERT_EQ(binary_operation_node->rhs->type, NodeType::NumberNode);
    ASSERT_EQ(binary_operation_node->token->type, TokenType::Multiply);

    auto* lhs = static_cast<BinaryOperationNode*>(binary_operation_node->lhs);
    ASSERT_EQ(lhs->lhs->type, NodeType::NumberNode);
    ASSERT_EQ(lhs->rhs->type, NodeType::NumberNode);
    ASSERT_EQ(lhs->token->type, TokenType::Plus);
}

TEST(Parser, ReturnNodeNoValue) {
    Lexer       lexer("test", "return");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);

    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);
    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::ReturnNode);
}

TEST(Parser, ReturnNodeWithValue) {
    Lexer       lexer("test", "return 123");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);

    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);
    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::ReturnNode);
    auto* return_node =
        static_cast<ReturnNode*>(statements_node->statements[0]);

    ASSERT_EQ(return_node->node_to_return->type, NodeType::NumberNode);
    ASSERT_EQ(return_node->node_to_return->token->value, "123");
}

TEST(Parser, ContinueNode) {
    Lexer       lexer("test", "continue");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);

    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);
    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::ContinueNode);
}

TEST(Parser, BreakNode) {
    Lexer       lexer("test", "break");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);

    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);
    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::BreakNode);
}

TEST(Parser, IncludeNode) {
    Lexer       lexer("test", "incl \"math\"");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();
    ASSERT_NE(parse_result.node, nullptr);

    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);
    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::IncludeNode);
    ASSERT_EQ(statements_node->statements[0]->token->value, "math");
}

TEST(Parser, VariableAssignment) {
    Lexer       lexer("test", "val $a is 123");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);
    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::VariableAssignmentNode);

    auto* var_def_node =
        static_cast<VariableAssignmentNode*>(statements_node->statements[0]);
    ASSERT_EQ(var_def_node->token->value, "$a");
    ASSERT_EQ(var_def_node->value->type, NodeType::NumberNode);
    ASSERT_EQ(var_def_node->value->token->value, "123");
}

TEST(Parser, VariableAssignmentWithChild) {
    Lexer       lexer("test", "val $a->$b is 123");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);
    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::VariableAssignmentNode);

    auto* var_def_node =
        static_cast<VariableAssignmentNode*>(statements_node->statements[0]);
    ASSERT_EQ(var_def_node->token->value, "$a");
    ASSERT_EQ(var_def_node->value->type, NodeType::NumberNode);
    ASSERT_EQ(var_def_node->value->token->value, "123");
    ASSERT_EQ(var_def_node->children.size(), 1);
    ASSERT_EQ(var_def_node->children[0].value, "$b");
}

TEST(Parser, VariableReassignmentNode) {
    Lexer       lexer("test", "$a is 123");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);
    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::VariableReassignmentNode);

    auto* var_reassign_node =
        static_cast<VariableReassignmentNode*>(statements_node->statements[0]);
    ASSERT_EQ(var_reassign_node->token->value, "$a");
    ASSERT_EQ(var_reassign_node->value->type, NodeType::NumberNode);
    ASSERT_EQ(var_reassign_node->value->token->value, "123");
}

TEST(Parser, VariableReassignmentWithChild) {
    Lexer       lexer("test", "$a->$b is 123");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);
    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::VariableReassignmentNode);

    auto* var_def_node =
        static_cast<VariableReassignmentNode*>(statements_node->statements[0]);
    ASSERT_EQ(var_def_node->token->value, "$a");
    ASSERT_EQ(var_def_node->value->type, NodeType::NumberNode);
    ASSERT_EQ(var_def_node->value->token->value, "123");
    ASSERT_EQ(var_def_node->children.size(), 1);
    ASSERT_EQ(var_def_node->children[0].value, "$b");
}

TEST(Parser, VariableAccessNode) {
    Lexer       lexer("test", "$a");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);
    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::VariableAccessNode);

    auto* var_access_node =
        static_cast<VariableAccessNode*>(statements_node->statements[0]);
    ASSERT_EQ(var_access_node->token->value, "$a");
}

TEST(Parser, VariableAccessWithChild) {
    Lexer       lexer("test", "$a->$b");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);
    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::VariableAccessNode);

    auto* var_access_node =
        static_cast<VariableAccessNode*>(statements_node->statements[0]);
    ASSERT_EQ(var_access_node->token->value, "$a");
    ASSERT_EQ(var_access_node->children.size(), 1);
    ASSERT_EQ(var_access_node->children[0].value, "$b");
}

TEST(Parser, LogicalNot) {
    Lexer       lexer("test", "!true");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);
    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::UnaryOperationNode);

    auto* unary_op_node =
        static_cast<UnaryOperationNode*>(statements_node->statements[0]);
    ASSERT_EQ(unary_op_node->token->type, TokenType::LogicalNot);
    ASSERT_EQ(unary_op_node->value->type, NodeType::BooleanNode);
    ASSERT_EQ(unary_op_node->value->token->value, "true");
}

TEST(Parser, NegativeNumbers) {
    Lexer       lexer("test", "-123");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);
    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::UnaryOperationNode);

    auto* unary_op_node =
        static_cast<UnaryOperationNode*>(statements_node->statements[0]);
    ASSERT_EQ(unary_op_node->token->type, TokenType::Minus);
    ASSERT_EQ(unary_op_node->value->type, NodeType::NumberNode);
    ASSERT_EQ(unary_op_node->value->token->value, "123");
}

TEST(Parser, PositiveNumber) {
    Lexer       lexer("test", "+123");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);
    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::UnaryOperationNode);

    auto* unary_op_node =
        static_cast<UnaryOperationNode*>(statements_node->statements[0]);
    ASSERT_EQ(unary_op_node->token->type, TokenType::Plus);
    ASSERT_EQ(unary_op_node->value->type, NodeType::NumberNode);
    ASSERT_EQ(unary_op_node->value->token->value, "123");
}

TEST(Parser, CallNoArgs) {
    Lexer       lexer("test", "func()");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);
    ASSERT_EQ(statements_node->statements[0]->type, NodeType::CallNode);

    auto* call_node = static_cast<CallNode*>(statements_node->statements[0]);
    ASSERT_EQ(call_node->node_to_call->type, NodeType::IdentifierNode);
    ASSERT_EQ(call_node->node_to_call->token->value, "func");
    ASSERT_EQ(call_node->arg_nodes.size(), 0);
}

TEST(Parser, CallWithArgs) {
    Lexer       lexer("test", "func(1, 2, 3)");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);
    ASSERT_EQ(statements_node->statements[0]->type, NodeType::CallNode);

    auto* call_node = static_cast<CallNode*>(statements_node->statements[0]);
    ASSERT_EQ(call_node->node_to_call->type, NodeType::IdentifierNode);
    ASSERT_EQ(call_node->node_to_call->token->value, "func");
    ASSERT_EQ(call_node->arg_nodes.size(), 3);
}

TEST(Parser, Identifier) {
    Lexer       lexer("test", "test");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);
    ASSERT_EQ(statements_node->statements[0]->type, NodeType::IdentifierNode);
    ASSERT_EQ(statements_node->statements[0]->token->value, "test");
}

TEST(Parser, BooleanTrue) {
    Lexer       lexer("test", "true; false");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 2);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::BooleanNode);
    ASSERT_EQ(statements_node->statements[0]->token->value, "true");

    ASSERT_EQ(statements_node->statements[1]->type, NodeType::BooleanNode);
    ASSERT_EQ(statements_node->statements[1]->token->value, "false");
}

TEST(Parser, EmptyList) {
    Lexer       lexer("test", "[]");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::ListNode);
    auto* list_node = static_cast<ListNode*>(statements_node->statements[0]);

    ASSERT_EQ(list_node->elements.size(), 0);
}

TEST(Parser, ListWithElements) {
    Lexer       lexer("test", "[1, 2, 3]");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::ListNode);
    auto* list_node = static_cast<ListNode*>(statements_node->statements[0]);

    ASSERT_EQ(list_node->elements.size(), 3);
    ASSERT_EQ(list_node->elements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(list_node->elements[0]->token->value, "1");
    ASSERT_EQ(list_node->elements[1]->type, NodeType::NumberNode);
    ASSERT_EQ(list_node->elements[1]->token->value, "2");
    ASSERT_EQ(list_node->elements[2]->type, NodeType::NumberNode);
    ASSERT_EQ(list_node->elements[2]->token->value, "3");
}

TEST(Parser, MultilineList) {
    Lexer       lexer("test", "[\n1,\n2,\n3\n]");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::ListNode);
    auto* list_node = static_cast<ListNode*>(statements_node->statements[0]);

    ASSERT_EQ(list_node->elements.size(), 3);
    ASSERT_EQ(list_node->elements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(list_node->elements[0]->token->value, "1");
    ASSERT_EQ(list_node->elements[1]->type, NodeType::NumberNode);
    ASSERT_EQ(list_node->elements[1]->token->value, "2");
    ASSERT_EQ(list_node->elements[2]->type, NodeType::NumberNode);
    ASSERT_EQ(list_node->elements[2]->token->value, "3");
}

TEST(Parser, EmptyDict) {
    Lexer       lexer("test", "{}");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();
    ASSERT_TRUE(lex_result.tokens.has_value());

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::DictNode);
    auto* list_node = static_cast<ListNode*>(statements_node->statements[0]);

    ASSERT_EQ(list_node->elements.size(), 0);
}

TEST(Parser, DictWithElements) {
    Lexer       lexer("test", "{\"A\" -> 1, \"B\" -> 2}");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::DictNode);
    auto* dict_node = static_cast<DictNode*>(statements_node->statements[0]);

    ASSERT_EQ(std::get<DICT_KEY>(dict_node->elements[0])->type,
              NodeType::StringNode);
    ASSERT_EQ(std::get<DICT_KEY>(dict_node->elements[0])->token->value, "A");
    ASSERT_EQ(std::get<DICT_VAL>(dict_node->elements[0])->type,
              NodeType::NumberNode);
    ASSERT_EQ(std::get<DICT_VAL>(dict_node->elements[0])->token->value, "1");

    ASSERT_EQ(std::get<DICT_KEY>(dict_node->elements[1])->type,
              NodeType::StringNode);
    ASSERT_EQ(std::get<DICT_KEY>(dict_node->elements[1])->token->value, "B");
    ASSERT_EQ(std::get<DICT_VAL>(dict_node->elements[1])->type,
              NodeType::NumberNode);
    ASSERT_EQ(std::get<DICT_VAL>(dict_node->elements[1])->token->value, "2");
}

TEST(Parser, MultilineDict) {
    Lexer       lexer("test", "{\n\"A\" -> 1,\n\"B\" -> 2\n}");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::DictNode);
    auto* dict_node = static_cast<DictNode*>(statements_node->statements[0]);

    ASSERT_EQ(dict_node->elements.size(), 2);

    ASSERT_EQ(std::get<DICT_KEY>(dict_node->elements[0])->type,
              NodeType::StringNode);
    ASSERT_EQ(std::get<DICT_KEY>(dict_node->elements[0])->token->value, "A");
    ASSERT_EQ(std::get<DICT_VAL>(dict_node->elements[0])->type,
              NodeType::NumberNode);
    ASSERT_EQ(std::get<DICT_VAL>(dict_node->elements[0])->token->value, "1");

    ASSERT_EQ(std::get<DICT_KEY>(dict_node->elements[1])->type,
              NodeType::StringNode);
    ASSERT_EQ(std::get<DICT_KEY>(dict_node->elements[1])->token->value, "B");
    ASSERT_EQ(std::get<DICT_VAL>(dict_node->elements[1])->type,
              NodeType::NumberNode);
    ASSERT_EQ(std::get<DICT_VAL>(dict_node->elements[1])->token->value, "2");
}

TEST(Parser, IfStatement) {
    Lexer       lexer("test", "if true = true { 1 }");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::IfNode);
    auto* if_node = static_cast<IfNode*>(statements_node->statements[0]);

    ASSERT_EQ(if_node->cases.size(), 1);
    ASSERT_EQ(std::get<IF_COND>(if_node->cases[0])->type,
              NodeType::BinaryOperationNode);
    ASSERT_TRUE(std::get<IF_RET>(if_node->cases[0]));

    auto* if_cond_0 =
        static_cast<StatementsNode*>(std::get<IF_EXPR>(if_node->cases[0]));
    ASSERT_EQ(if_cond_0->type, NodeType::StatementsNode);
    ASSERT_EQ(if_cond_0->statements.size(), 1);
    ASSERT_EQ(if_cond_0->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(if_cond_0->statements[0]->token->value, "1");
}

TEST(Parser, IfStatementMultiline) {
    Lexer       lexer("test", "if true = true {\n1\n}");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::IfNode);
    auto* if_node = static_cast<IfNode*>(statements_node->statements[0]);

    ASSERT_EQ(if_node->cases.size(), 1);
    ASSERT_EQ(std::get<IF_COND>(if_node->cases[0])->type,
              NodeType::BinaryOperationNode);
    ASSERT_TRUE(std::get<IF_RET>(if_node->cases[0]));

    auto* if_cond_0 =
        static_cast<StatementsNode*>(std::get<IF_EXPR>(if_node->cases[0]));
    ASSERT_EQ(if_cond_0->type, NodeType::StatementsNode);
    ASSERT_EQ(if_cond_0->statements.size(), 1);
    ASSERT_EQ(if_cond_0->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(if_cond_0->statements[0]->token->value, "1");
}

TEST(Parser, IfElseIfStatement) {
    Lexer       lexer("test", "if true = true { 1 } else if true = true { 2 }");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::IfNode);
    auto* if_node = static_cast<IfNode*>(statements_node->statements[0]);

    ASSERT_EQ(if_node->cases.size(), 2);

    ASSERT_EQ(std::get<IF_COND>(if_node->cases[0])->type,
              NodeType::BinaryOperationNode);
    ASSERT_TRUE(std::get<IF_RET>(if_node->cases[0]));

    auto* if_cond_0 =
        static_cast<StatementsNode*>(std::get<IF_EXPR>(if_node->cases[0]));
    ASSERT_EQ(if_cond_0->type, NodeType::StatementsNode);
    ASSERT_EQ(if_cond_0->statements.size(), 1);
    ASSERT_EQ(if_cond_0->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(if_cond_0->statements[0]->token->value, "1");

    ASSERT_EQ(std::get<IF_COND>(if_node->cases[1])->type,
              NodeType::BinaryOperationNode);
    ASSERT_TRUE(std::get<IF_RET>(if_node->cases[1]));

    auto* if_cond_1 =
        static_cast<StatementsNode*>(std::get<IF_EXPR>(if_node->cases[1]));
    ASSERT_EQ(if_cond_1->type, NodeType::StatementsNode);
    ASSERT_EQ(if_cond_1->statements.size(), 1);
    ASSERT_EQ(if_cond_1->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(if_cond_1->statements[0]->token->value, "2");
}

TEST(Parser, MultilineIfElseIfStatement) {
    Lexer lexer("test", "if true = true {\n1\n} else if true = true {\n2\n}");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::IfNode);
    auto* if_node = static_cast<IfNode*>(statements_node->statements[0]);

    ASSERT_EQ(if_node->cases.size(), 2);

    ASSERT_EQ(std::get<IF_COND>(if_node->cases[0])->type,
              NodeType::BinaryOperationNode);
    ASSERT_TRUE(std::get<IF_RET>(if_node->cases[0]));

    auto* if_cond_0 =
        static_cast<StatementsNode*>(std::get<IF_EXPR>(if_node->cases[0]));
    ASSERT_EQ(if_cond_0->type, NodeType::StatementsNode);
    ASSERT_EQ(if_cond_0->statements.size(), 1);
    ASSERT_EQ(if_cond_0->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(if_cond_0->statements[0]->token->value, "1");

    ASSERT_EQ(std::get<IF_COND>(if_node->cases[1])->type,
              NodeType::BinaryOperationNode);
    ASSERT_TRUE(std::get<IF_RET>(if_node->cases[1]));

    auto* if_cond_1 =
        static_cast<StatementsNode*>(std::get<IF_EXPR>(if_node->cases[1]));
    ASSERT_EQ(if_cond_1->type, NodeType::StatementsNode);
    ASSERT_EQ(if_cond_1->statements.size(), 1);
    ASSERT_EQ(if_cond_1->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(if_cond_1->statements[0]->token->value, "2");
}

TEST(Parser, IfElseStatement) {
    Lexer       lexer("test", "if true = true { 1 } else { 3 }");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::IfNode);
    auto* if_node = static_cast<IfNode*>(statements_node->statements[0]);

    ASSERT_EQ(if_node->cases.size(), 1);

    ASSERT_EQ(std::get<IF_COND>(if_node->cases[0])->type,
              NodeType::BinaryOperationNode);
    ASSERT_TRUE(std::get<IF_RET>(if_node->cases[0]));

    auto* if_cond_0 =
        static_cast<StatementsNode*>(std::get<IF_EXPR>(if_node->cases[0]));
    ASSERT_EQ(if_cond_0->type, NodeType::StatementsNode);
    ASSERT_EQ(if_cond_0->statements.size(), 1);
    ASSERT_EQ(if_cond_0->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(if_cond_0->statements[0]->token->value, "1");

    ASSERT_TRUE(std::get<IF_RET - 1>(if_node->else_case.value()));

    auto* else_cond = static_cast<StatementsNode*>(
        std::get<IF_EXPR - 1>(if_node->else_case.value()));
    ASSERT_EQ(else_cond->type, NodeType::StatementsNode);
    ASSERT_EQ(else_cond->statements.size(), 1);
    ASSERT_EQ(else_cond->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(else_cond->statements[0]->token->value, "3");
}

TEST(Parser, MultilineIfElseStatement) {
    Lexer       lexer("test", "if true = true {\n1\n} else {\n3\n}");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::IfNode);
    auto* if_node = static_cast<IfNode*>(statements_node->statements[0]);

    ASSERT_EQ(if_node->cases.size(), 1);

    ASSERT_EQ(std::get<IF_COND>(if_node->cases[0])->type,
              NodeType::BinaryOperationNode);
    ASSERT_TRUE(std::get<IF_RET>(if_node->cases[0]));

    auto* if_cond_0 =
        static_cast<StatementsNode*>(std::get<IF_EXPR>(if_node->cases[0]));
    ASSERT_EQ(if_cond_0->type, NodeType::StatementsNode);
    ASSERT_EQ(if_cond_0->statements.size(), 1);
    ASSERT_EQ(if_cond_0->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(if_cond_0->statements[0]->token->value, "1");

    ASSERT_TRUE(std::get<IF_RET - 1>(if_node->else_case.value()));

    auto* else_cond = static_cast<StatementsNode*>(
        std::get<IF_EXPR - 1>(if_node->else_case.value()));
    ASSERT_EQ(else_cond->type, NodeType::StatementsNode);
    ASSERT_EQ(else_cond->statements.size(), 1);
    ASSERT_EQ(else_cond->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(else_cond->statements[0]->token->value, "3");
}

TEST(Parser, IfElseIfElseStatement) {
    Lexer       lexer("test",
                      "if true = true { 1 } else if true = true { 2 } else { 3 }");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::IfNode);
    auto* if_node = static_cast<IfNode*>(statements_node->statements[0]);

    ASSERT_EQ(if_node->cases.size(), 2);

    ASSERT_EQ(std::get<IF_COND>(if_node->cases[0])->type,
              NodeType::BinaryOperationNode);
    ASSERT_TRUE(std::get<IF_RET>(if_node->cases[0]));

    auto* if_cond_0 =
        static_cast<StatementsNode*>(std::get<IF_EXPR>(if_node->cases[0]));
    ASSERT_EQ(if_cond_0->type, NodeType::StatementsNode);
    ASSERT_EQ(if_cond_0->statements.size(), 1);
    ASSERT_EQ(if_cond_0->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(if_cond_0->statements[0]->token->value, "1");

    ASSERT_EQ(std::get<IF_COND>(if_node->cases[1])->type,
              NodeType::BinaryOperationNode);
    ASSERT_TRUE(std::get<IF_RET>(if_node->cases[1]));

    auto* if_cond_1 =
        static_cast<StatementsNode*>(std::get<IF_EXPR>(if_node->cases[1]));
    ASSERT_EQ(if_cond_1->type, NodeType::StatementsNode);
    ASSERT_EQ(if_cond_1->statements.size(), 1);
    ASSERT_EQ(if_cond_1->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(if_cond_1->statements[0]->token->value, "2");

    ASSERT_TRUE(std::get<IF_RET - 1>(if_node->else_case.value()));

    auto* else_cond = static_cast<StatementsNode*>(
        std::get<IF_EXPR - 1>(if_node->else_case.value()));
    ASSERT_EQ(else_cond->type, NodeType::StatementsNode);
    ASSERT_EQ(else_cond->statements.size(), 1);
    ASSERT_EQ(else_cond->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(else_cond->statements[0]->token->value, "3");
}

TEST(Parser, MultilineIfElseIfElseStatement) {
    Lexer lexer(
        "test",
        "if true = true {\n1\n} else if true = true {\n2\n} else {\n3\n}");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::IfNode);
    auto* if_node = static_cast<IfNode*>(statements_node->statements[0]);

    ASSERT_EQ(if_node->cases.size(), 2);

    ASSERT_EQ(std::get<IF_COND>(if_node->cases[0])->type,
              NodeType::BinaryOperationNode);
    ASSERT_TRUE(std::get<IF_RET>(if_node->cases[0]));

    auto* if_cond_0 =
        static_cast<StatementsNode*>(std::get<IF_EXPR>(if_node->cases[0]));
    ASSERT_EQ(if_cond_0->type, NodeType::StatementsNode);
    ASSERT_EQ(if_cond_0->statements.size(), 1);
    ASSERT_EQ(if_cond_0->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(if_cond_0->statements[0]->token->value, "1");

    ASSERT_EQ(std::get<IF_COND>(if_node->cases[1])->type,
              NodeType::BinaryOperationNode);
    ASSERT_TRUE(std::get<IF_RET>(if_node->cases[1]));

    auto* if_cond_1 =
        static_cast<StatementsNode*>(std::get<IF_EXPR>(if_node->cases[1]));
    ASSERT_EQ(if_cond_1->type, NodeType::StatementsNode);
    ASSERT_EQ(if_cond_1->statements.size(), 1);
    ASSERT_EQ(if_cond_1->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(if_cond_1->statements[0]->token->value, "2");

    ASSERT_TRUE(std::get<IF_RET - 1>(if_node->else_case.value()));

    auto* else_cond = static_cast<StatementsNode*>(
        std::get<IF_EXPR - 1>(if_node->else_case.value()));
    ASSERT_EQ(else_cond->type, NodeType::StatementsNode);
    ASSERT_EQ(else_cond->statements.size(), 1);
    ASSERT_EQ(else_cond->statements[0]->type, NodeType::NumberNode);
    ASSERT_EQ(else_cond->statements[0]->token->value, "3");
}

TEST(Parser, ForLoop) {
    Lexer       lexer("test", "for $i in 0..10 { 1 }");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::ForNode);
    auto* for_node = static_cast<ForNode*>(statements_node->statements[0]);

    ASSERT_EQ(for_node->token->value, "$i");
    ASSERT_EQ(for_node->start_value->type, NodeType::NumberNode);
    ASSERT_EQ(for_node->end_value->type, NodeType::NumberNode);
    ASSERT_EQ(for_node->step_value, nullptr);
    ASSERT_TRUE(for_node->should_return_value);

    ASSERT_EQ(for_node->body->type, NodeType::StatementsNode);
    auto* body_statements = static_cast<StatementsNode*>(for_node->body);

    ASSERT_EQ(body_statements->statements.size(), 1);
    ASSERT_EQ(body_statements->statements[0]->type, NodeType::NumberNode);
}

TEST(Parser, ForLoopWithStep) {
    Lexer       lexer("test", "for $i in 0..10..2 { 1 }");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::ForNode);
    auto* for_node = static_cast<ForNode*>(statements_node->statements[0]);

    ASSERT_EQ(for_node->token->value, "$i");
    ASSERT_EQ(for_node->start_value->type, NodeType::NumberNode);
    ASSERT_EQ(for_node->end_value->type, NodeType::NumberNode);
    ASSERT_EQ(for_node->step_value->type, NodeType::NumberNode);
    ASSERT_TRUE(for_node->should_return_value);

    ASSERT_EQ(for_node->body->type, NodeType::StatementsNode);
    auto* body_statements = static_cast<StatementsNode*>(for_node->body);

    ASSERT_EQ(body_statements->statements.size(), 1);
    ASSERT_EQ(body_statements->statements[0]->type, NodeType::NumberNode);
}

TEST(Parser, WhileLoop) {
    Lexer       lexer("test", "while true { 1 }");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type, NodeType::WhileNode);
    auto* while_node = static_cast<WhileNode*>(statements_node->statements[0]);

    ASSERT_EQ(while_node->condition->type, NodeType::BooleanNode);
    ASSERT_EQ(while_node->condition->token->value, "true");

    ASSERT_EQ(while_node->body->type, NodeType::StatementsNode);
    auto* body_node = static_cast<StatementsNode*>(while_node->body);

    ASSERT_EQ(body_node->statements.size(), 1);
    ASSERT_EQ(body_node->statements[0]->type, NodeType::NumberNode);
}

TEST(Parser, FunctionDefinitionNoArgs) {
    Lexer       lexer("test", "fn test() { 1 }");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::FunctionDefinitionNode);
    auto* function_node =
        static_cast<FunctionDefinitionNode*>(statements_node->statements[0]);

    ASSERT_TRUE(function_node->name.has_value());
    ASSERT_EQ(function_node->name.value().value, "test");
    ASSERT_EQ(function_node->arg_names.size(), 0);
    ASSERT_TRUE(function_node->should_auto_return);
    ASSERT_TRUE(function_node->should_add_to_table);

    ASSERT_NE(function_node->body_node, nullptr);
    ASSERT_EQ(function_node->body_node->type, NodeType::StatementsNode);

    auto* function_body =
        static_cast<StatementsNode*>(function_node->body_node);
    ASSERT_EQ(function_body->statements.size(), 1);
    ASSERT_EQ(function_body->statements[0]->type, NodeType::NumberNode);
}

TEST(Parser, FunctionDefinitionWithArgs) {
    Lexer       lexer("test", "fn add($a, $b) { $a + $b }");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::FunctionDefinitionNode);
    auto* function_node =
        static_cast<FunctionDefinitionNode*>(statements_node->statements[0]);

    ASSERT_TRUE(function_node->name.has_value());
    ASSERT_EQ(function_node->name.value().value, "add");
    ASSERT_EQ(function_node->arg_names.size(), 2);
    ASSERT_TRUE(function_node->should_auto_return);
    ASSERT_TRUE(function_node->should_add_to_table);

    ASSERT_NE(function_node->body_node, nullptr);
    ASSERT_EQ(function_node->body_node->type, NodeType::StatementsNode);

    auto* function_body =
        static_cast<StatementsNode*>(function_node->body_node);
    ASSERT_EQ(function_body->statements.size(), 1);
    ASSERT_EQ(function_body->statements[0]->type,
              NodeType::BinaryOperationNode);
}

TEST(Parser, FunctionDefinitionNoArgsMultiline) {
    Lexer       lexer("test", "fn test() {\n1\n}");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::FunctionDefinitionNode);
    auto* function_node =
        static_cast<FunctionDefinitionNode*>(statements_node->statements[0]);

    ASSERT_TRUE(function_node->name.has_value());
    ASSERT_EQ(function_node->name.value().value, "test");
    ASSERT_EQ(function_node->arg_names.size(), 0);
    ASSERT_TRUE(function_node->should_auto_return);
    ASSERT_TRUE(function_node->should_add_to_table);

    ASSERT_NE(function_node->body_node, nullptr);
    ASSERT_EQ(function_node->body_node->type, NodeType::StatementsNode);

    auto* function_body =
        static_cast<StatementsNode*>(function_node->body_node);
    ASSERT_EQ(function_body->statements.size(), 1);
    ASSERT_EQ(function_body->statements[0]->type, NodeType::NumberNode);
}

TEST(Parser, FunctionDefinitionWithArgsMultiline) {
    Lexer       lexer("test", "fn add($a, $b) {\n$a + $b\n}");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::FunctionDefinitionNode);
    auto* function_node =
        static_cast<FunctionDefinitionNode*>(statements_node->statements[0]);

    ASSERT_TRUE(function_node->name.has_value());
    ASSERT_EQ(function_node->name.value().value, "add");
    ASSERT_EQ(function_node->arg_names.size(), 2);
    ASSERT_TRUE(function_node->should_auto_return);
    ASSERT_TRUE(function_node->should_add_to_table);

    ASSERT_NE(function_node->body_node, nullptr);
    ASSERT_EQ(function_node->body_node->type, NodeType::StatementsNode);

    auto* function_body =
        static_cast<StatementsNode*>(function_node->body_node);
    ASSERT_EQ(function_body->statements.size(), 1);
    ASSERT_EQ(function_body->statements[0]->type,
              NodeType::BinaryOperationNode);
}

TEST(Parser, ClassNoConstructor) {
    Lexer lexer("test", "class Test {\nfn hello() {\nprint(\"Hello!\")\n}\n}");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::ClassDefinitionNode);
    auto* class_node =
        static_cast<ClassDefinitionNode*>(statements_node->statements[0]);

    ASSERT_EQ(class_node->token->value, "Test");

    ASSERT_EQ(class_node->body_node->type, NodeType::StatementsNode);
    auto* body_node = static_cast<StatementsNode*>(class_node->body_node);

    ASSERT_EQ(body_node->statements.size(), 1);
    ASSERT_EQ(body_node->statements[0]->type, NodeType::FunctionDefinitionNode);
}

TEST(Parser, ClassWithConstructorNoArgs) {
    Lexer lexer(
        "test",
        "class Test {\nfn __constructor() {\n$this->$a is 5;\n}\n\nfn hello() {\nprint(\"Hello!\")\n}\n}");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::ClassDefinitionNode);
    auto* class_node =
        static_cast<ClassDefinitionNode*>(statements_node->statements[0]);

    ASSERT_EQ(class_node->token->value, "Test");

    ASSERT_EQ(class_node->body_node->type, NodeType::StatementsNode);
    auto* body_node = static_cast<StatementsNode*>(class_node->body_node);

    ASSERT_EQ(body_node->statements.size(), 2);
    ASSERT_EQ(body_node->statements[0]->type, NodeType::FunctionDefinitionNode);
    ASSERT_EQ(body_node->statements[1]->type, NodeType::FunctionDefinitionNode);
}

TEST(Parser, ClassWithConstructorArgs) {
    Lexer lexer(
        "test",
        "class Person {\nfn __constructor($name, $age) {\n$this->$name is $name;\n$this->$age is $age;\n}\nfn hello() {\nprint(\"Hello! I am \" + $this->$name + \"!\")\n}\nfn birthday() {\n$this->$age is $this->age + 1\nprint(\"Happy birthday, \" + $this->$name + \"!\")\n}\n}");
    LexerResult lex_result = lexer.tokenize();

    ASSERT_FALSE(lex_result.error.has_value())
        << "Error: " << lex_result.error.value().str();

    Parser      parser(lex_result.tokens.value());
    ParseResult parse_result = parser.parse();

    ASSERT_FALSE(parse_result.error.has_value())
        << "Error: " << parse_result.error.value().str();

    ASSERT_NE(parse_result.node, nullptr);
    ASSERT_EQ(parse_result.node->type, NodeType::StatementsNode);

    auto* statements_node = static_cast<StatementsNode*>(parse_result.node);
    ASSERT_EQ(statements_node->statements.size(), 1);

    ASSERT_EQ(statements_node->statements[0]->type,
              NodeType::ClassDefinitionNode);
    auto* class_node =
        static_cast<ClassDefinitionNode*>(statements_node->statements[0]);

    ASSERT_EQ(class_node->token->value, "Person");

    ASSERT_EQ(class_node->body_node->type, NodeType::StatementsNode);
    auto* body_node = static_cast<StatementsNode*>(class_node->body_node);

    ASSERT_EQ(body_node->statements.size(), 3);
    ASSERT_EQ(body_node->statements[0]->type, NodeType::FunctionDefinitionNode);
    ASSERT_EQ(body_node->statements[1]->type, NodeType::FunctionDefinitionNode);

    auto* constructor_node =
        static_cast<FunctionDefinitionNode*>(body_node->statements[0]);
    ASSERT_EQ(constructor_node->arg_names.size(), 2);
}
