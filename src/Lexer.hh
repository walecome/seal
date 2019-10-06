#pragma once

#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

#include "Token.hh"
#include "TokenBuffer.hh"

class Lexer {
   public:
    Lexer(std::string_view source) : source { source } {}

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

    void reportError(const std::string_view value) const;

    std::string_view cut(unsigned start, unsigned steps) const;

    std::pair<TokenType, std::string_view> findLongestMatchingToken();

    const std::string_view source;
    unsigned current_index { 0 };
    TokenBuffer tokens {};
    unsigned row { 0 };
    unsigned col { 0 };
};