#pragma once

#include <iostream>
#include "nodes.hpp"
#include "types/object.hpp"
#include "context.hpp"

struct RuntimeResult {
    std::optional<Object *> value;
    std::optional<BaseError> error;
    std::optional<Object *> function_return_value;
    bool loop_should_continue = false;
    bool loop_should_break = false;

    void reset() {
        this->function_return_value = std::nullopt;
        this->loop_should_continue = false;
        this->loop_should_break = false;
    }

    std::optional<Object *> reg(RuntimeResult result) {
        this->error = result.error;
        this->function_return_value = result.function_return_value;
        this->loop_should_continue = result.loop_should_continue;
        this->loop_should_break = result.loop_should_break;
        return result.value;
    }

    RuntimeResult *success(Object *value) {
        this->reset();
        this->value = value;

        return this;
    }

    RuntimeResult *success_return(Object *value) {
        this->reset();
        this->function_return_value = value;

        return this;
    }

    RuntimeResult *success_continue() {
        this->reset();
        this->loop_should_continue = true;

        return this;
    }

    RuntimeResult *success_break() {
        this->reset();
        this->loop_should_break = true;

        return this;
    }

    RuntimeResult *failure(BaseError error) {
        this->reset();
        this->error = error;

        return this;
    }

    bool should_return() const {
        return this->error.has_value() ||
               this->function_return_value.has_value() ||
               this->loop_should_continue ||
               this->loop_should_break;
    }
};

class Interpreter {
public:
    RuntimeResult visit(BaseNode *node, Context &context);

private:
    RuntimeResult visit_NumberNode(NumberNode *node, Context &context);

    RuntimeResult visit_BooleanNode(BooleanNode *node, Context &context);

    RuntimeResult visit_StringNode(StringNode *node, Context &context);

    RuntimeResult visit_ListNode(ListNode *node, Context &context);

    RuntimeResult visit_DictNode(DictNode *node, Context &context);

    RuntimeResult visit_IdentifierNode(IdentifierNode *node, Context &context);

    RuntimeResult visit_VariableAssignmentNode(VariableAssignmentNode *node, Context &context);

    RuntimeResult visit_VariableAccessNode(VariableAccessNode *node, Context &context);

    RuntimeResult visit_VariableReassignmentNode(VariableReassignmentNode *node, Context &context);

    RuntimeResult visit_BinaryOperationNode(BinaryOperationNode *node, Context &context);

    RuntimeResult visit_UnaryOperationNode(UnaryOperationNode *node, Context &context);

    RuntimeResult visit_FunctionDefinitionNode(FunctionDefinitionNode *node, Context &context);

    RuntimeResult visit_StatementsNode(StatementsNode *node, Context &context);

    RuntimeResult visit_CallNode(CallNode *node, Context &context);

    RuntimeResult visit_ReturnNode(ReturnNode *node, Context &context);

    RuntimeResult visit_ContinueNode(ContinueNode *node, Context &context);

    RuntimeResult visit_BreakNode(BreakNode *node, Context &context);

    RuntimeResult visit_IfNode(IfNode *node, Context &context);

    RuntimeResult visit_ForNode(ForNode *node, Context &context);

    RuntimeResult visit_WhileNode(WhileNode *node, Context &context);

    RuntimeResult visit_ClassDefinitionNode(ClassDefinitionNode *node, Context &context);
};
