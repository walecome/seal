#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "TokenBuffer.hh"
#include "parse/Block.hh"
#include "parse/Function.hh"
#include "parse/FunctionDecl.hh"
#include "parse/Node.hh"
#include "parse/Parameter.hh"
#include "parse/ParameterList.hh"
#include "parse/Type.hh"

class Parser {
   public:
    void parse(TokenBuffer &tokens);

    ptr_t<Function> parseFunction(TokenBuffer &tokens);
    ptr_t<FunctionDecl> parseFunctionDecl(TokenBuffer &tokens);

    ptr_t<Parameter> parseParameter(TokenBuffer &tokens);
    ptr_t<ParameterList> parseParameterList(TokenBuffer &tokens);

    ptr_t<Block> parseBlock(TokenBuffer &tokens);

    void expect(TokenBuffer &tokens, TokenType tokenType);

    std::vector<ptr_t<Node>> parseNodes {};
};