#pragma once

#include <cassert>
#include <functional>
#include <iomanip>
#include <memory>
#include <stack>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <variant>

#include "Scope.hh"
#include "ast/ArgumentList.hh"
#include "ast/ArrayLiteral.hh"
#include "ast/AssignExpression.hh"
#include "ast/BinaryExpression.hh"
#include "ast/Block.hh"
#include "ast/BooleanLiteral.hh"
#include "ast/CompareExpression.hh"
#include "ast/CompilationUnit.hh"
#include "ast/EqualityExpression.hh"
#include "ast/FunctionCall.hh"
#include "ast/FunctionDecl.hh"
#include "ast/IfStatement.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/Literal.hh"
#include "ast/Operator.hh"
#include "ast/ParameterList.hh"
#include "ast/ReturnStatement.hh"
#include "ast/StringLiteral.hh"
#include "ast/Type.hh"
#include "ast/UnaryExpression.hh"
#include "ast/VariableDecl.hh"
#include "ast/VariableExpression.hh"
#include "ast/While.hh"

using expr_value_t = std::variant<int, float, bool, std::string>;

struct Environment {
    Environment *parent { nullptr };

    void set_variable(std::string_view ident, expr_value_t data,
                      bool force_local);
    expr_value_t get_variable(std::string_view ident) const;
    Environment *env_of_variable(std::string_view ident) const;

    std::unordered_map<std::string_view, expr_value_t> variable_data {};
};

struct ReturnException : public std::exception {
    ReturnException(expr_value_t value) : value { value } {}
    expr_value_t value;
};

struct EnvGuard;

struct Interpreter {
    Interpreter(Scope *function_scope) : function_scope { function_scope } {}

    void interpret_argument_list(ArgumentList *);
    void interpret_array_literal(ArrayLiteral *);
    expr_value_t interpret_assign_expr(AssignExpression *);
    expr_value_t interpret_binary_expr(BinaryExpression *);
    void interpret_block(Block *);
    expr_value_t interpret_compare_expr(CompareExpression *);
    void interpret_compilation_unit(CompilationUnit *);
    expr_value_t interpret_expr(Expression *);
    expr_value_t interpret_equality_expr(EqualityExpression *);
    expr_value_t interpret_function_call(FunctionCall *);
    void interpret_if_statement(IfStatement *);
    expr_value_t interpret_literal(Literal *);
    void interpret_operator(Operator *);
    void interpret_param_list(ParameterList *);
    void interpret_return(ReturnStatement *);
    void interpret_statement(Statement *);
    void interpret_type(Type *);
    expr_value_t interpret_unary_expr(UnaryExpression *);
    void interpret_variable_decl(VariableDecl *);
    expr_value_t interpret_variable_expr(VariableExpression *);
    void interpret_while(While *);

    void handle_print(Expression *);

    Environment *current_env();
    void _enter_block();
    void _exit_block();

    EnvGuard acquire_block();

    std::stack<ptr_t<Environment>> environments {};
    Scope *function_scope;

    int number_enter = 0;
    int number_exit = 0;
};

struct EnvGuard {
    EnvGuard() = delete;
    EnvGuard(Interpreter *interpreter) : interpreter { interpreter } {
        interpreter->_enter_block();
    }

    ~EnvGuard() { interpreter->_exit_block(); }
    Interpreter *interpreter;
};
