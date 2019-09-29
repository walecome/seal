#pragma once

#include <iostream>
#include <memory>
#include <stack>
#include <stdexcept>
#include <vector>

#include "Error.hh"
#include "TokenBuffer.hh"
#include "ast/ArgumentList.hh"
#include "ast/ArrayLiteral.hh"
#include "ast/BinaryExpression.hh"
#include "ast/Block.hh"
#include "ast/Expression.hh"
#include "ast/Function.hh"
#include "ast/FunctionCall.hh"
#include "ast/FunctionDecl.hh"
#include "ast/IfStatement.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/Node.hh"
#include "ast/Parameter.hh"
#include "ast/ParameterList.hh"
#include "ast/ReturnStatement.hh"
#include "ast/Statement.hh"
#include "ast/StringLiteral.hh"
#include "ast/Type.hh"
#include "ast/VariableDecl.hh"
#include "ast/VariableExpression.hh"
#include "ast/While.hh"

class Parser {
   public:
    Parser(bool verbose) : verbose { verbose } {}

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

    bool verbose;

    std::vector<ptr_t<Node>> parseNodes {};
};