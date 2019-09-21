#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>

#include "TokenBuffer.hh"
#include "parse/Block.hh"
#include "parse/Function.hh"
#include "parse/FunctionDecl.hh"

template <typename T>
using ptr = std::unique_ptr<T>;

class Parser {
   public:
    void parse(TokenBuffer &tokens);

    Function *parseFunction(TokenBuffer &tokens);
    FunctionDecl *parseFunctionDecl(TokenBuffer &tokens);

    Block *parseBlock(TokenBuffer &tokens);

    void expect(TokenBuffer &tokens, TokenType tokenType);
};