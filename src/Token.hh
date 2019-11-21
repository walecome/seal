#pragma once

#include <iostream>
#include <iterator>
#include <sstream>
#include <string_view>
#include <unordered_map>

#include "ast/Type.hh"

#define ENUMERATE_TOKENS()          \
    __ENUMERATE_TOKEN(UNKNOWN)      \
    __ENUMERATE_TOKEN(EOF_TOKEN)    \
    __ENUMERATE_TOKEN(LPARENS)      \
    __ENUMERATE_TOKEN(RPARENS)      \
    __ENUMERATE_TOKEN(LARGPARENS)   \
    __ENUMERATE_TOKEN(RARGPARENS)   \
    __ENUMERATE_TOKEN(HASH)         \
    __ENUMERATE_TOKEN(ARROW)        \
    __ENUMERATE_TOKEN(LT)           \
    __ENUMERATE_TOKEN(GT)           \
    __ENUMERATE_TOKEN(LTEQ)         \
    __ENUMERATE_TOKEN(GTEQ)         \
    __ENUMERATE_TOKEN(ASSIGN)       \
    __ENUMERATE_TOKEN(EQ)           \
    __ENUMERATE_TOKEN(NOT_EQ)       \
    __ENUMERATE_TOKEN(SLASH)        \
    __ENUMERATE_TOKEN(LBRACKET)     \
    __ENUMERATE_TOKEN(RBRACKET)     \
    __ENUMERATE_TOKEN(LBRACE)       \
    __ENUMERATE_TOKEN(RBRACE)       \
    __ENUMERATE_TOKEN(MINUS)        \
    __ENUMERATE_TOKEN(PLUS)         \
    __ENUMERATE_TOKEN(STAR)         \
    __ENUMERATE_TOKEN(MODULO)       \
    __ENUMERATE_TOKEN(INC)          \
    __ENUMERATE_TOKEN(DEC)          \
    __ENUMERATE_TOKEN(AMP)          \
    __ENUMERATE_TOKEN(PIPE)         \
    __ENUMERATE_TOKEN(AMP_AMP)      \
    __ENUMERATE_TOKEN(PIPE_PIPE)    \
    __ENUMERATE_TOKEN(COMMA)        \
    __ENUMERATE_TOKEN(DOT)          \
    __ENUMERATE_TOKEN(TILDE)        \
    __ENUMERATE_TOKEN(SINGLE_QUOTE) \
    __ENUMERATE_TOKEN(DOUBLE_QUOTE) \
    __ENUMERATE_TOKEN(EXCL)         \
    __ENUMERATE_TOKEN(QUESTION)     \
    __ENUMERATE_TOKEN(POWER)        \
    __ENUMERATE_TOKEN(COLON)        \
    __ENUMERATE_TOKEN(SEMICOLON)    \
    __ENUMERATE_TOKEN(IDENTIFIER)   \
    __ENUMERATE_TOKEN(NUMBER)       \
    __ENUMERATE_TOKEN(STRING)       \
    __ENUMERATE_TOKEN(UNEXPECTED)   \
    __ENUMERATE_TOKEN(FUNC_KEYWORD) \
    __ENUMERATE_TOKEN(RETURN)       \
    __ENUMERATE_TOKEN(IF)           \
    __ENUMERATE_TOKEN(ELSE)         \
    __ENUMERATE_TOKEN(ELSE_IF)      \
    __ENUMERATE_TOKEN(WHILE)        \
    __ENUMERATE_TOKEN(FOR)          \
    __ENUMERATE_TOKEN(TYPE)         \
    __ENUMERATE_TOKEN(BOOL)         \
    __ENUMERATE_TOKEN(FUNC_CALL)    \
    __ENUMERATE_TOKEN(MUTABLE)

enum TokenType {
#undef __ENUMERATE_TOKEN
#define __ENUMERATE_TOKEN(token) token,
    ENUMERATE_TOKENS()
#undef __ENUMERATE_TOKEN

};

static std::unordered_map<TokenType, std::string_view> token_names {
#undef __ENUMERATE_TOKEN
#define __ENUMERATE_TOKEN(token) { token, #token },
    ENUMERATE_TOKENS()
#undef __ENUMERATE_TOKEN
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
    { "^", POWER },         { "++", INC },    { "--", DEC }
};

static std::unordered_map<std::string_view, TokenType> keyword_map {
    { "fn", FUNC_KEYWORD }, { "if", IF },       { "else", ELSE },
    { "elsif", ELSE_IF },   { "while", WHILE }, { "return", RETURN },
    { "mut", MUTABLE },     { "true", BOOL },   { "false", BOOL },
    { "for", FOR }
};

struct Token {
    Token() : row { 0 }, col { 0 } {}
    Token(unsigned row, unsigned col)
        : type { TokenType::UNKNOWN }, row { row }, col { col } {}
    Token(unsigned row, unsigned col, TokenType token_type)
        : type { token_type }, row { row }, col { col } {}

    std::string to_string() const;

    TokenType type;
    unsigned row;
    unsigned col;

    std::string_view value {};
};

TokenType string_to_token(const std::string_view s);
bool is_type(const std::string_view s);
void try_replace_keyword_or_type(Token &token);