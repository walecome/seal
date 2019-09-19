#pragma once

#include <memory>
#include <stdexcept>

#include "TokenBuffer.hh"

template <typename T>
using ptr = std::unique_ptr<T>;

struct ParseNode {};

class Parser {
   public:
    void parse(TokenBuffer &tokens);

    ParseNode *parseFunction(TokenBuffer &tokens);
    ParseNode *parseFunctionDecl(TokenBuffer &tokens);

    ParseNode *parseBlock(TokenBuffer &tokens);

    void expect(TokenBuffer &tokens, TokenType tokenType);
};