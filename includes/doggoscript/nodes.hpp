#pragma once

#include <string>
#include <format>
#include <utility>
#include <variant>
#include <unordered_map>
#include "token.hpp"
#include "position.hpp"

enum class NodeType {
    NumberNode,                 // Done
    BooleanNode,                // Done
    IdentifierNode,             // Done
    StringNode,                 // Done
    ListNode,                   // Done
    StatementsNode,             // Done
    BinaryOperationNode,        // Done
    UnaryOperationNode,         // Done
    VariableAssignmentNode,     // Done
    VariableReassignmentNode,   // Done
    VariableAccessNode,         // Done
    IfNode,                     // Done
    ForNode,                    // Done
    WhileNode,                  // Done
    FunctionDefinitionNode,     // Done
    CallNode,                   // Done
    ReturnNode,                 // Done
    ContinueNode,               // Done
    BreakNode,                  // Done
};

const std::unordered_map<NodeType, std::string> node_type_to_str = {
    { NodeType::NumberNode, "NumberNode" },
    { NodeType::BooleanNode, "BooleanNode" },
    { NodeType::IdentifierNode, "IdentifierNode" },
    { NodeType::StringNode, "StringNode" },
    { NodeType::ListNode, "ListNode" },
    { NodeType::StatementsNode, "StatementsNode" },
    { NodeType::BinaryOperationNode, "BinaryOperationNode" },
    { NodeType::UnaryOperationNode, "UnaryOperationNode" },
    { NodeType::VariableAssignmentNode, "VariableAssignmentNode" },
    { NodeType::VariableReassignmentNode, "VariableReassignmentNode" },
    { NodeType::VariableAccessNode, "VariableAccessNode" },
    { NodeType::IfNode, "IfNode" },
    { NodeType::ForNode, "ForNode" },
    { NodeType::WhileNode, "WhileNode" },
    { NodeType::FunctionDefinitionNode, "FunctionDefinitionNode" },
    { NodeType::CallNode, "CallNode" },
    { NodeType::ReturnNode, "ReturnNode" },
    { NodeType::ContinueNode, "ContinueNode" },
    { NodeType::BreakNode, "BreakNode" },
};

struct BaseNode {
    NodeType type;
    Token* token;
    Position* start_pos;
    Position* end_pos;
};

struct NumberNode : public BaseNode {
    explicit NumberNode(Token& token) : BaseNode() {
        this->type = NodeType::NumberNode;
        this->token = new Token(token);
        this->start_pos = &this->token->start_pos.value();
        this->end_pos = &this->token->end_pos.value();
    }
};

struct BooleanNode : public BaseNode {
    explicit BooleanNode(Token& token) : BaseNode() {
        this->type = NodeType::BooleanNode;
        this->token = new Token(token);
        this->start_pos = &this->token->start_pos.value();
        this->end_pos = &this->token->end_pos.value();
    }
};

struct IdentifierNode : public BaseNode {
    explicit IdentifierNode(Token& token) : BaseNode() {
        this->type = NodeType::IdentifierNode;
        this->token = new Token(token);
        this->start_pos = &this->token->start_pos.value();
        this->end_pos = &this->token->end_pos.value();
    }
};

struct StringNode : public BaseNode {
    explicit StringNode(Token& token) : BaseNode() {
        this->type = NodeType::StringNode;
        this->token = new Token(token);
        this->start_pos = &this->token->start_pos.value();
        this->end_pos = &this->token->end_pos.value();
    }
};

struct ListNode : public BaseNode {
    std::vector<BaseNode*> elements;

    explicit ListNode(std::vector<BaseNode*> elements, Position &start_pos, Position &end_pos) : BaseNode() {
        this->type = NodeType::ListNode;
        this->elements = std::move(elements);
        this->start_pos = new Position(start_pos);
        this->end_pos = new Position(end_pos);
    }
};

struct StatementsNode : public BaseNode {
    std::vector<BaseNode*> statements;

    explicit StatementsNode(std::vector<BaseNode*> statements, Position &start_pos, Position &end_pos) : BaseNode() {
        this->type = NodeType::StatementsNode;
        this->statements = std::move(statements);
        this->start_pos = new Position(start_pos);
        this->end_pos = new Position(end_pos);
    }
};

struct BinaryOperationNode : public BaseNode {
    BaseNode* lhs;
    BaseNode* rhs;

    explicit BinaryOperationNode(BaseNode* lhs, Token& operation, BaseNode* rhs) : BaseNode() {
        this->type = NodeType::BinaryOperationNode;
        this->token = new Token(operation);
        this->lhs = lhs;
        this->rhs = rhs;
        this->start_pos = lhs->start_pos;
        this->end_pos = rhs->end_pos;
    }
};

struct UnaryOperationNode : public BaseNode {
    BaseNode* value;

    explicit UnaryOperationNode(Token& operation, BaseNode* value) : BaseNode() {
        this->type = NodeType::UnaryOperationNode;
        this->token = new Token(operation);
        this->value = value;
        this->start_pos = &this->token->start_pos.value();
        this->end_pos = value->end_pos;
    }
};

struct VariableAssignmentNode : public BaseNode {
    BaseNode* value;

    explicit VariableAssignmentNode(Token& name, BaseNode* value) : BaseNode() {
        this->type = NodeType::VariableAssignmentNode;
        this->token = new Token(name);
        this->value = value;
        this->start_pos = &this->token->start_pos.value();
        this->end_pos = value->end_pos;
    }
};

struct VariableReassignmentNode : public BaseNode {
    BaseNode* value;

    explicit VariableReassignmentNode(Token& name, BaseNode* value) : BaseNode() {
        this->type = NodeType::VariableReassignmentNode;
        this->token = new Token(name);
        this->value = value;
        this->start_pos = &this->token->start_pos.value();
        this->end_pos = value->end_pos;
    }
};

struct VariableAccessNode : public BaseNode {
    explicit VariableAccessNode(Token& name) : BaseNode() {
        this->type = NodeType::VariableAccessNode;
        this->token = new Token(name);
        this->start_pos = &this->token->start_pos.value();
        this->end_pos = &this->token->end_pos.value();
    }
};

struct IfNode : public BaseNode {
    std::vector<std::tuple<BaseNode*, BaseNode*, bool>> cases;
    std::optional<std::tuple<BaseNode*, bool>> else_case;

    IfNode(std::vector<std::tuple<BaseNode*, BaseNode*, bool>> cases, std::optional<std::tuple<BaseNode*, bool>> else_case = std::nullopt) : BaseNode() {
        this->type = NodeType::IfNode;
        this->cases = cases;
        this->else_case = else_case;
        this->start_pos = std::get<0>(cases[0])->start_pos;

        if(else_case.has_value())
            this->end_pos = std::get<0>(else_case.value())->end_pos;
        else
            this->end_pos = std::get<0>(cases[cases.size() - 1])->end_pos;
    }
};

struct ForNode : public BaseNode {
    BaseNode* start_value;
    BaseNode* end_value;
    BaseNode* step_value;
    BaseNode* body;
    bool should_return_value;

    ForNode(Token& var_name, BaseNode* start_value, BaseNode* end_value, BaseNode* step_value, BaseNode* body, bool should_return_value) : BaseNode() {
        this->type = NodeType::ForNode;
        this->token = new Token(var_name);
        this->start_value = start_value;
        this->end_value = end_value;
        this->step_value = step_value;
        this->body = body;
        this->should_return_value = should_return_value;
        this->start_pos = &this->token->start_pos.value();
        this->end_pos = body->end_pos;
    }
};

struct WhileNode : public BaseNode {
    BaseNode* condition;
    BaseNode* body;
    bool should_return_value;

    WhileNode(BaseNode* condition, BaseNode* body, bool should_return_value) : BaseNode() {
        this->type = NodeType::WhileNode;
        this->condition = condition;
        this->body = body;
        this->start_pos = condition->start_pos;
        this->end_pos = body->end_pos;
        this->should_return_value = should_return_value;
    }
};

struct FunctionDefinitionNode : public BaseNode {
    std::optional<Token> name;
    std::vector<Token*> arg_names;
    BaseNode* body_node;
    bool should_auto_return;

    FunctionDefinitionNode(std::optional<Token> &name, std::vector<Token*> arg_names, BaseNode* body_node, bool should_auto_return) : BaseNode() {
        this->type = NodeType::FunctionDefinitionNode;
        this->name = name;
        this->arg_names = std::move(arg_names);
        this->body_node = body_node;
        this->should_auto_return = should_auto_return;

        if(this->name.has_value()) {
            this->start_pos = &this->name.value().start_pos.value();
        } else if(!this->arg_names.empty()) {
            this->start_pos = &this->arg_names[0]->start_pos.value();
        } else {
            this->start_pos = body_node->start_pos;
        }

        this->end_pos = body_node->end_pos;
    }
};

struct CallNode : public BaseNode {
    BaseNode* node_to_call;
    std::vector<BaseNode*> arg_nodes;

    CallNode(BaseNode* node_to_call, std::vector<BaseNode*> arg_nodes) : BaseNode() {
        this->type = NodeType::CallNode;
        this->node_to_call = node_to_call;
        this->arg_nodes = std::move(arg_nodes);
        this->start_pos = node_to_call->start_pos;
        if(!this->arg_nodes.empty())
            this->end_pos = this->arg_nodes[this->arg_nodes.size() - 1]->end_pos;
        else
            this->end_pos = this->node_to_call->end_pos;
    }
};

struct ReturnNode : public BaseNode {
    BaseNode* node_to_return;

    ReturnNode(BaseNode* node_to_return, Position &start_pos, Position &end_pos) : BaseNode() {
        this->type = NodeType::ReturnNode;
        this->node_to_return = node_to_return;
        this->start_pos = new Position(start_pos);
        this->end_pos = new Position(end_pos);
    }
};

struct ContinueNode : public BaseNode {
    ContinueNode(Position &start_pos, Position &end_pos) : BaseNode() {
        this->type = NodeType::ContinueNode;
        this->start_pos = new Position(start_pos);
        this->end_pos = new Position(end_pos);
    }
};

struct BreakNode : public BaseNode {
    BreakNode(Position &start_pos, Position &end_pos) : BaseNode() {
        this->type = NodeType::BreakNode;
        this->start_pos = new Position(start_pos);
        this->end_pos = new Position(end_pos);
    }
};
