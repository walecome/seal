#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Token.hh"
#include "TokenBuffer.hh"

class Lexer {
   public:
    Lexer(std::string source) : source { source } {}

    void readAll();
    Token readOne();

    unsigned numberOfTokens() const { return tokens.size(); }

    // Get lexer tokens as reference
    TokenBuffer &getTokens() { return tokens; }

   private:
    void readWhitespace();
    void readAlpha(Token &token);
    void readNumber(Token &token);
    void readString(Token &token, char string_opener);
    void readSymbol(Token &token);

    void reportError(const std::string &value) const;

    std::string cut(unsigned start, unsigned steps) const;

    std::pair<TokenType, std::string> findLongestMatchingToken();

    const std::string source;
    unsigned current_index { 0 };
    TokenBuffer tokens {};
};