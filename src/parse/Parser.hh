#pragma once

#include <iostream>
#include <memory>
#include <stack>
#include <stdexcept>
#include <vector>

#include "TokenBuffer.hh"

#include "ast/Block.hh"
#include "ast/Expression.hh"
#include "ast/Function.hh"
#include "ast/FunctionDecl.hh"
#include "ast/Node.hh"
#include "ast/Parameter.hh"
#include "ast/ParameterList.hh"
#include "ast/Type.hh"

class Parser {
   public:
    void parse(TokenBuffer &tokens);

    ptr_t<Function> parseFunction(TokenBuffer &tokens);
    ptr_t<FunctionDecl> parseFunctionDecl(TokenBuffer &tokens);

    ptr_t<Parameter> parseParameter(TokenBuffer &tokens);
    ptr_t<ParameterList> parseParameterList(TokenBuffer &tokens);

    ptr_t<Block> parseBlock(TokenBuffer &tokens);

    ptr_t<Expression> parseExpression(TokenBuffer &tokens);

    std::vector<Token> shuntingYard(TokenBuffer &tokens);
    ptr_t<Expression> rpnToExpressions(TokenBuffer &tokens);

    std::vector<ptr_t<Node>> parseNodes {};
};