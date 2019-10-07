#pragma once

#include <iostream>
#include <memory>
#include <stack>
#include <stdexcept>
#include <vector>

#include "Constants.hh"
#include "Error.hh"
#include "TokenBuffer.hh"
#include "ast/CompilationUnit.hh"

struct ArgumentList;
struct ArrayLiteral;
struct BinaryExpression;
struct Block;
struct Expression;
struct Function;
struct FunctionCall;
struct FunctionDecl;
struct IfStatement;
struct IntegerLiteral;
struct Node;
struct ParameterList;
struct ReturnStatement;
struct Statement;
struct StringLiteral;
struct Type;
struct VariableDecl;
struct VariableExpression;
struct While;

class Parser {
   public:
    Parser() = default;

    void parse(TokenBuffer &tokens);

    ptr_t<ArgumentList> parse_argument_list(TokenBuffer &tokens);
    ptr_t<Block> parse_block(TokenBuffer &tokens);
    ptr_t<Expression> parse_expression(TokenBuffer &tokens);
    ptr_t<Expression> rpn_to_expressions(TokenBuffer &tokens);
    ptr_t<Function> parse_function(TokenBuffer &tokens);
    ptr_t<FunctionDecl> parse_function_decl(TokenBuffer &tokens);
    ptr_t<IfStatement> parse_if_statement(TokenBuffer &tokens);
    ptr_t<VariableDecl> parse_parameter(TokenBuffer &tokens);
    ptr_t<ParameterList> parse_parameter_list(TokenBuffer &tokens);
    ptr_t<Statement> parse_statement(TokenBuffer &tokens);
    ptr_t<VariableDecl> parse_variable_decl(TokenBuffer &tokens);
    ptr_t<While> parse_while(TokenBuffer &tokens);
    ptr_t<ReturnStatement> parse_return_statement(TokenBuffer &tokens);
    TokenBuffer shunting_yard(TokenBuffer &tokens);
    ptr_t<ArrayLiteral> parse_array_literal(TokenBuffer &tokens);

    ptr_t<CompilationUnit> compilation_unit { nullptr };
};