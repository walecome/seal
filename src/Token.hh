#pragma once

#include <iostream>
#include <iterator>
#include <sstream>
#include <string_view>
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
    FUNC_KEYWORD,
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

static std::unordered_map<std::string_view, TokenType> string_token_map {
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

static std::unordered_map<std::string_view, TokenType> keyword_map {
    { "func", FUNC_KEYWORD }, { "if", IF },       { "else", ELSE },
    { "elsif", ELSE_IF },     { "while", WHILE }, { "return", RETURN },
};

static std::unordered_map<TokenType, std::string_view> token_names {
    REFLECT_TOKEN(AMP_AMP),      REFLECT_TOKEN(AMP),
    REFLECT_TOKEN(ARROW),        REFLECT_TOKEN(COLON),
    REFLECT_TOKEN(COMMA),        REFLECT_TOKEN(DOT),
    REFLECT_TOKEN(DOUBLE_QUOTE), REFLECT_TOKEN(EQ),
    REFLECT_TOKEN(EXCL),         REFLECT_TOKEN(FUNC_KEYWORD),
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
    Token() : row { 0 }, col { 0 } {}
    Token(unsigned row, unsigned col) : row { row }, col { col } {}
    TokenType type;

    std::string_view value {};

    std::string to_string() const;

    unsigned row;
    unsigned col;
};

TokenType string_to_token(const std::string_view s);
bool is_type(const std::string_view s);
void try_replace_keyword_or_type(Token &token);