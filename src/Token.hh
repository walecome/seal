#pragma once

#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>

enum class TokenType {
    LPARENS,
    RPARENS,
    IDENTIFIER,
    NUMBER,
    STRING,
    UNEXPECTED
};

struct Token {
    Token() = default;
    TokenType type;

    std::string value {};

    void print() const;
};

static std::unordered_map<std::string, TokenType> stringTokenMap {
    { "(", TokenType::LPARENS },
    { ")", TokenType::RPARENS },
};

TokenType stringToToken(const std::string &s);