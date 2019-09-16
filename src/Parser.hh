#pragma once

#include <stdexcept>

#include "TokenBuffer.hh"

class Parser {
   public:
    void parse(TokenBuffer &tokens);

    void parseFunction(TokenBuffer &tokens);
    void parseFunctionDefinition(TokenBuffer &tokens);
    void parseFunctionBody(TokenBuffer &tokens);

    void expect(TokenBuffer &tokens, TokenType tokenType);
};