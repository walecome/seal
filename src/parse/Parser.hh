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
struct Parameter;
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

    ptr_t<ArgumentList> parseArgumentList(TokenBuffer &tokens);
    ptr_t<Block> parseBlock(TokenBuffer &tokens);
    ptr_t<Expression> parseExpression(TokenBuffer &tokens);
    ptr_t<Expression> rpnToExpressions(TokenBuffer &tokens);
    ptr_t<Function> parseFunction(TokenBuffer &tokens);
    ptr_t<FunctionDecl> parseFunctionDecl(TokenBuffer &tokens);
    ptr_t<IfStatement> parseIfStatement(TokenBuffer &tokens);
    ptr_t<Parameter> parseParameter(TokenBuffer &tokens);
    ptr_t<ParameterList> parseParameterList(TokenBuffer &tokens);
    ptr_t<Statement> parseStatement(TokenBuffer &tokens);
    ptr_t<VariableDecl> parseVariableDecl(TokenBuffer &tokens);
    ptr_t<While> parseWhile(TokenBuffer &tokens);
    ptr_t<ReturnStatement> parseReturnStatement(TokenBuffer &tokens);
    TokenBuffer shuntingYard(TokenBuffer &tokens);
    ptr_t<ArrayLiteral> parseArrayLiteral(TokenBuffer &tokens);

    ptr_t<CompilationUnit> compilationUnit { nullptr };
};