#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Token.hh"

class Lexer {
   public:
    Lexer(std::string source) : source { source } {}

    void readAll();
    Token readOne();

    void printTokens() const;

   private:
    void readWhitespace();
    void readAlpha(Token &token);
    void readNumber(Token &token);
    void readString(Token &token);
    void readSymbol(Token &token);

    std::string cut(unsigned start, unsigned steps) const;

    std::pair<TokenType, std::string> findLongestMatchingToken();

    const std::string source;
    unsigned current_index { 0 };
    std::vector<Token> tokens {};
};