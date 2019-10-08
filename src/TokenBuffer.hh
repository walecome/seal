#pragma once

#include <sstream>
#include <unordered_map>
#include <vector>

#include "Token.hh"

class TokenBuffer {
   public:
    TokenBuffer() = default;
    TokenBuffer(std::vector<Token> from_tokens) : tokens { from_tokens } {}

    bool accept(TokenType token_type) const;
    bool can_pop(TokenType type, int offset = 0);
    bool eat(const TokenType token_type);
    bool empty() const;

    size_t size() const { return tokens.size(); }

    std::string dump() const;
    Token pop();
    Token top() const { return tokens[index]; };

    void add_token(const Token token);
    void backtrack(unsigned steps);
    void expect(TokenType token_type);
    void print_tokens() const;
    std::string reconstruct_row(unsigned row) const;

   private:
    unsigned index { 0 };

    std::vector<Token> tokens {};
    std::unordered_map<unsigned, unsigned> row_indices { { 0, 0 } };

    unsigned previous_row { 0 };
};
