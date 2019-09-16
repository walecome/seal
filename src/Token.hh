#pragma once

#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>

enum class TokenType {
    LPARENS,
    RPARENS,
    HASH,
    ARROW,
    LT,
    GT,
    LTEQ,
    GTEQ,
    EQ,
    SLASH,
    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,
    MINUS,
    PLUS,
    STAR,
    MODULO,

    AMP,
    PIPE,

    AMP_AMP,
    PIPE_PIPE,

    COMMA,
    DOT,

    TILDE,

    SINGLE_QUOTE,
    DOUBLE_QUOTE,

    EXCL,
    QUESTION,

    POWER,

    COLON,
    SEMICOLON,

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
    { "(", TokenType::LPARENS },      { ")", TokenType::RPARENS },
    { "#", TokenType::HASH },         { "->", TokenType::ARROW },
    { "<", TokenType::LT },           { ">", TokenType::GT },
    { "<=", TokenType::LTEQ },        { ">=", TokenType::GTEQ },
    { "/", TokenType::SLASH },        { "[", TokenType::LBRACKET },
    { "]", TokenType::RBRACKET },     { "{", TokenType::LBRACE },
    { "}", TokenType::RBRACE },       { "-", TokenType::MINUS },
    { "+", TokenType::PLUS },         { "*", TokenType::STAR },
    { "%", TokenType::MODULO },       { "&", TokenType::AMP },
    { "|", TokenType::PIPE },         { ".", TokenType::DOT },
    { ",", TokenType::COMMA },        { ":", TokenType::COLON },
    { ";", TokenType::SEMICOLON },    { "=", TokenType::EQ },
    { "'", TokenType::SINGLE_QUOTE }, { "\"", TokenType::DOUBLE_QUOTE },
    { "!", TokenType::EXCL },         { "&&", TokenType::AMP_AMP },
    { "||", TokenType::PIPE_PIPE },   { "~", TokenType::TILDE },
    { "?", TokenType::QUESTION },     { "^", TokenType::POWER },
};

TokenType stringToToken(const std::string &s);