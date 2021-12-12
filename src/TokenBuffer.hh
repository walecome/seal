#pragma once

#include <initializer_list>
#include <iterator>
#include <optional>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Token.hh"

struct SourceRef;

class TokenBuffer {
   public:
    using Iterator = std::vector<Token>::iterator;

    TokenBuffer() = default;
    TokenBuffer(std::vector<Token> from_tokens) : tokens { from_tokens } {}

    bool accept(TokenType token_type) const;
    bool can_pop(TokenType type, int offset = 0);
    std::optional<Token> eat(const TokenType token_type);
    bool eat(const std::initializer_list<TokenType> eatable_tokens);
    bool empty() const;

    size_t size() const { return tokens.size(); }

    std::string dump() const;
    Token pop();
    Token top() const { return tokens[index]; };
    Token previous() const { return tokens[index - 1]; }

    Iterator top_iterator();

    void add_token(const Token token);
    void backtrack(unsigned steps);
    Token expect(TokenType token_type);
    void print_tokens() const;
    std::string reconstruct_row(unsigned row) const;

    static TokenBuffer source_tokens(SourceRef source_ref);

    // Prints the row of the token that is currently pointed to, as well as a
    // column indication. If error_message is provided this will be printed
    // after the source row.
    // @TODO: Consider if this should be moved somewhere else.
    void dump_current_row(const std::string_view error_message = "") const;

    std::string as_source() const;

   private:
    unsigned index { 0 };

    std::vector<Token> tokens {};
    std::unordered_map<unsigned, unsigned> row_indices { { 0, 0 } };

    unsigned previous_row { 0 };
};
