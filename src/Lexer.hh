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

    void read_all();

    unsigned number_of_tokens() const { return tokens.size(); }

    // Get lexer tokens as reference
    TokenBuffer &get_tokens() { return tokens; }

   private:
    Token read_one();

    void read_whitespace();
    void read_comment();
    void read_alpha(Token &token);
    void read_number(Token &token);
    void read_string(Token &token, char string_opener);
    void read_symbol(Token &token);

    void report_error(const std::string_view value) const;

    void reset_row();
    void reset_col();

    std::string_view cut(unsigned start, unsigned steps) const;

    std::pair<TokenType, std::string_view> find_longest_matching_token();

    const std::string_view source;
    unsigned current_index { 0 };
    TokenBuffer tokens {};
    unsigned row { 1 };
    unsigned col { 1 };
};