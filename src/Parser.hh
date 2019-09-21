#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "TokenBuffer.hh"
#include "parse/Block.hh"
#include "parse/Function.hh"
#include "parse/FunctionDecl.hh"
#include "parse/Parameter.hh"
#include "parse/ParameterList.hh"
#include "parse/Type.hh"

template <typename T>
using ptr = std::unique_ptr<T>;

class Parser {
   public:
    ~Parser();

    void parse(TokenBuffer &tokens);

    Function *parseFunction(TokenBuffer &tokens);
    FunctionDecl *parseFunctionDecl(TokenBuffer &tokens);

    Parameter *parseParameter(TokenBuffer &tokens);
    ParameterList *parseParameterList(TokenBuffer &tokens);

    Block *parseBlock(TokenBuffer &tokens);

    void expect(TokenBuffer &tokens, TokenType tokenType);

    std::vector<Node *> parseNodes {};
};