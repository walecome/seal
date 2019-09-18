#pragma once

#include <memory>
#include <stdexcept>

#include "TokenBuffer.hh"

template <typename T>
using ptr = std::unique_ptr<T>;

struct ParseNode {
    ParseNode() = default;
};

class Parser {
   public:
    void parse(TokenBuffer &tokens);

    ParseNode *parseFunction(TokenBuffer &tokens);
    ParseNode *parseFunctionDecl(TokenBuffer &tokens);

    void parseBlock(TokenBuffer &tokens);

    void expect(TokenBuffer &tokens, TokenType tokenType);
};