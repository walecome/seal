#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <stack>
#include <string_view>
#include <unordered_map>
#include <variant>

#include "Scope.hh"
#include "ast/ArgumentList.hh"
#include "ast/ArrayLiteral.hh"
#include "ast/AssignExpression.hh"
#include "ast/BinaryExpression.hh"
#include "ast/Block.hh"
#include "ast/CompareExpression.hh"
#include "ast/CompilationUnit.hh"
#include "ast/EqualityExpression.hh"
#include "ast/Function.hh"
#include "ast/FunctionCall.hh"
#include "ast/FunctionDecl.hh"
#include "ast/IfStatement.hh"
#include "ast/Literal.hh"
#include "ast/Operator.hh"
#include "ast/ParameterList.hh"
#include "ast/ReturnStatement.hh"
#include "ast/Type.hh"
#include "ast/UnaryExpression.hh"
#include "ast/VariableDecl.hh"
#include "ast/VariableExpression.hh"
#include "ast/While.hh"

using expr_value_t = std::variant<int, float, bool, std::string>;

struct Environment {
    Environment *parent { nullptr };

    void set_variable(std::string_view ident, expr_value_t data);
    expr_value_t get_variable(std::string_view ident);

    std::unordered_map<std::string_view, expr_value_t> variable_data {};
};

struct Interpreter {
    Interpreter() = default;

    void interpret_argument_list(ArgumentList *);
    void interpret_array_literal(ArrayLiteral *);
    expr_value_t interpret_assign_expr(AssignExpression *);
    expr_value_t interpret_binary_expr(BinaryExpression *);
    void interpret_block(Block *);
    expr_value_t interpret_compare_expr(CompareExpression *);
    void interpret_compilation_unit(CompilationUnit *);
    expr_value_t interpret_expr(Expression *);
    expr_value_t interpret_equality_expr(EqualityExpression *);
    void interpret_function(Function *);
    expr_value_t interpret_function_call(FunctionCall *);
    void interpret_if_statement(IfStatement *);
    void interpret_literal(Literal *);
    void interpret_operator(Operator *);
    void interpret_param_list(ParameterList *);
    void interpret_return(ReturnStatement *);
    void interpret_statement(Statement *);
    void interpret_type(Type *);
    expr_value_t interpret_unary_expr(UnaryExpression *);
    void interpret_variable_decl(VariableDecl *);
    expr_value_t interpret_variable_expr(VariableExpression *);
    void interpret_while(While *);

    Environment *current_env();
    void enter_block();
    void exit_block();

    std::stack<ptr_t<Environment>> environments {};
};