#pragma once

#include <tuple>
#include <vector>
#include <functional>
#include "object.hpp"
#include "../error.hpp"
#include "../context.hpp"
#include "../interpreter.hpp"
#include "../nodes.hpp"

enum class FunctionType {
    UserDefined,
    BuiltIn
};

struct BaseFunction : public Object {
    std::string name;
    FunctionType func_type;
    explicit BaseFunction(std::string name) : name(name), Object() { this->type = ObjectType::Function; }

    std::string str() {
        return "<function " + this->name + ">";
    }

    Context generate_new_context() {
        Context context("<function " + this->name + ">");
        context.symbol_table = new SymbolTable(this->context->symbol_table);
        context.parent = this->context;
        return context;
    }

    RuntimeResult check_args(std::vector<std::string> arg_names, std::vector<Object*> args) {
        RuntimeResult result;
        if(args.size() != arg_names.size()) {
            return *result.failure(ArgumentError(
                *this->start_pos, *this->end_pos,
                "Expected " + std::to_string(arg_names.size()) + " arguments but got " + std::to_string(args.size()) + "!"
            ));
        }

        return *result.success(nullptr);
    }

    static void populate_args(std::vector<std::string> arg_names, std::vector<Object*> args, Context& exec_ctx) {
        for(int i = 0; i < args.size(); i++) {
            std::string arg_name = arg_names[i];
            Object* arg_value = args[i];
            arg_value->set_context(&exec_ctx);
            exec_ctx.symbol_table->set(arg_name, arg_value);
        }
    }

    RuntimeResult check_and_populate_args(std::vector<std::string> arg_names, std::vector<Object*> args, Context& exec_ctx) {
        RuntimeResult result;
        result.reg(this->check_args(arg_names, args));
        if(result.error.has_value()) return result;

        this->populate_args(arg_names, args, exec_ctx);
        return *result.success(nullptr);
    }

    virtual RuntimeResult operator()(std::vector<Object*> args) = 0;
};

struct Function : public BaseFunction {
    BaseNode* body_node;
    std::vector<std::string> arg_names;
    bool should_auto_return;

    Function(std::string name, BaseNode* body_node, std::vector<std::string> arg_names, bool should_auto_return)
        : BaseFunction(name), body_node(body_node), arg_names(arg_names), should_auto_return(should_auto_return) { this->func_type = FunctionType::UserDefined; }

    RuntimeResult operator()(std::vector<Object*> args) override {
        RuntimeResult result;
        Interpreter interpreter;
        Context exec_ctx = this->generate_new_context();

        result.reg(this->check_and_populate_args(this->arg_names, args, exec_ctx));
        if(result.error.has_value()) return result;

        std::optional<Object*> value = result.reg(interpreter.visit(this->body_node, exec_ctx));
        if(result.should_return() && !result.function_return_value) {
            return result;
        }

        Object* return_value;

        if(this->should_auto_return) {
            return_value = value.value();
        } else if(result.function_return_value.has_value()) {
            return_value = result.function_return_value.value();
        } else {
            return_value = nullptr;
        }

        return *result.success(return_value);
    }
};

struct BuiltInFunction : public BaseFunction {
    std::vector<std::string> arg_names;
    std::function<RuntimeResult(std::vector<Object*>)> func;

    BuiltInFunction(std::string name, std::vector<std::string> arg_names, std::function<RuntimeResult(std::vector<Object*>)> func)
        : BaseFunction(name), arg_names(arg_names), func(func) { this->func_type = FunctionType::BuiltIn; }

    std::string str() override {
        return "<built-in function " + this->name + ">";
    }

    RuntimeResult operator()(std::vector<Object*> args) override {
        RuntimeResult result;
        result.reg(this->check_args(this->arg_names, args));
        if(result.error.has_value()) return result;

        return this->func(args);
    }
};
