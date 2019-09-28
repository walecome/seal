#pragma once

#include <sstream>
#include <vector>

// #include "Error.hh"
#include "Token.hh"

class TokenBuffer {
   public:
    TokenBuffer() = default;
    TokenBuffer(std::vector<Token> fromTokens) : tokens { fromTokens } {}

    void addToken(const Token token);

    void printTokens() const;
    std::string dump() const;

    size_t size() const { return tokens.size(); }

    Token pop();

    bool eat(const TokenType tokenType);

    Token top() const { return tokens[index]; };

    bool empty() const;

    bool accept(TokenType tokenType) const;

    void expect(TokenType tokenType);

    bool canPop(TokenType type, int offset = 0);

   private:
    unsigned index { 0 };

    std::vector<Token> tokens {};
};
