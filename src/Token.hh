#pragma once

#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ast/Type.hh"

#define REFLECT_TOKEN(token) \
    { (token), #token }

enum TokenType {
    LPARENS,
    RPARENS,
    LARGPARENS,
    RARGPARENS,

    HASH,
    ARROW,

    LT,
    GT,
    LTEQ,
    GTEQ,
    ASSIGN,
    EQ,
    NOT_EQ,
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

    UNEXPECTED,

    // Keywords
    FUNC_IDENT,
    RETURN,
    IF,
    ELSE,
    ELSE_IF,
    WHILE,

    // Type
    TYPE,

    // Semantic
    FUNC_CALL
};

static std::unordered_map<std::string, TokenType> stringTokenMap {
    { "(", LPARENS },       { ")", RPARENS }, { "#", HASH },
    { "->", ARROW },        { "<", LT },      { ">", GT },
    { "<=", LTEQ },         { ">=", GTEQ },   { "!=", NOT_EQ },
    { "=", ASSIGN },        { "/", SLASH },   { "[", LBRACKET },
    { "]", RBRACKET },      { "{", LBRACE },  { "}", RBRACE },
    { "-", MINUS },         { "+", PLUS },    { "*", STAR },
    { "%", MODULO },        { "&", AMP },     { "|", PIPE },
    { ".", DOT },           { ",", COMMA },   { ":", COLON },
    { ";", SEMICOLON },     { "==", EQ },     { "'", SINGLE_QUOTE },
    { "\"", DOUBLE_QUOTE }, { "!", EXCL },    { "&&", AMP_AMP },
    { "||", PIPE_PIPE },    { "~", TILDE },   { "?", QUESTION },
    { "^", POWER },
};

static std::unordered_map<std::string, TokenType> keywordMap {
    { "func", FUNC_IDENT }, { "if", IF },       { "else", ELSE },
    { "elsif", ELSE_IF },   { "while", WHILE }, { "return", RETURN },
};

static std::unordered_map<TokenType, std::string> tokenNames {
    REFLECT_TOKEN(AMP_AMP),      REFLECT_TOKEN(AMP),
    REFLECT_TOKEN(ARROW),        REFLECT_TOKEN(COLON),
    REFLECT_TOKEN(COMMA),        REFLECT_TOKEN(DOT),
    REFLECT_TOKEN(DOUBLE_QUOTE), REFLECT_TOKEN(EQ),
    REFLECT_TOKEN(EXCL),         REFLECT_TOKEN(FUNC_IDENT),
    REFLECT_TOKEN(GT),           REFLECT_TOKEN(GTEQ),
    REFLECT_TOKEN(HASH),         REFLECT_TOKEN(IDENTIFIER),
    REFLECT_TOKEN(LBRACE),       REFLECT_TOKEN(LBRACKET),
    REFLECT_TOKEN(LPARENS),      REFLECT_TOKEN(LT),
    REFLECT_TOKEN(LTEQ),         REFLECT_TOKEN(MINUS),
    REFLECT_TOKEN(MODULO),       REFLECT_TOKEN(NUMBER),
    REFLECT_TOKEN(PIPE_PIPE),    REFLECT_TOKEN(PIPE),
    REFLECT_TOKEN(PLUS),         REFLECT_TOKEN(POWER),
    REFLECT_TOKEN(QUESTION),     REFLECT_TOKEN(RBRACE),
    REFLECT_TOKEN(RBRACKET),     REFLECT_TOKEN(RPARENS),
    REFLECT_TOKEN(SEMICOLON),    REFLECT_TOKEN(SINGLE_QUOTE),
    REFLECT_TOKEN(SLASH),        REFLECT_TOKEN(STAR),
    REFLECT_TOKEN(STRING),       REFLECT_TOKEN(TILDE),
    REFLECT_TOKEN(TYPE),         REFLECT_TOKEN(UNEXPECTED),
    REFLECT_TOKEN(FUNC_CALL),    REFLECT_TOKEN(ASSIGN),
    REFLECT_TOKEN(RETURN),       REFLECT_TOKEN(IF),
    REFLECT_TOKEN(ELSE),         REFLECT_TOKEN(ELSE_IF),
    REFLECT_TOKEN(WHILE),

};

struct Token {
    Token(unsigned row, unsigned col) : row { row }, col { col } {}
    TokenType type;

    std::string value {};

    std::string toString() const;

    unsigned row;
    unsigned col;
};

TokenType stringToToken(const std::string &s);

bool isType(const std::string &s);

void tryReplaceKeywordOrType(Token &token);