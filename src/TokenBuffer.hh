#pragma once

#include <vector>

#include "Token.hh"

class TokenBuffer {
   public:
    void addToken(const Token token);

    void printTokens() const;

    size_t size() const { return tokens.size(); }

    Token pop();

    bool eat(const TokenType tokenType);

    bool empty() const;

   private:
    unsigned index { 0 };

    std::vector<Token> tokens {};
};
