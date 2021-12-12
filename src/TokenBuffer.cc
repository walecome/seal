#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

#include "Error.hh"
#include "SourceRef.hh"
#include "TokenBuffer.hh"

void TokenBuffer::add_token(const Token token) {
    if (token.row > previous_row) {
        previous_row = token.row;
        row_indices.insert({ token.row, tokens.size() });
    }
    tokens.push_back(token);
}

void TokenBuffer::print_tokens() const { std::cout << dump() << std::endl; }

std::string TokenBuffer::dump() const {
    std::ostringstream oss {};
    oss << "Tokens (" << tokens.size() << "):" << std::endl;
    for (auto token : tokens) {
        oss << "\t" << token.to_string() << std::endl;
    }

    return oss.str();
}

std::optional<Token> TokenBuffer::eat(const TokenType token_type) {
    Token target = tokens.at(index);
    if (target.type == token_type) {
        ++index;
        return target;
    }

    return {};
}

bool TokenBuffer::eat(const std::initializer_list<TokenType> eatable_tokens) {
    for (auto& token : eatable_tokens) {
        if (eat(token)) return true;
    }

    return false;
}

bool TokenBuffer::empty() const { return index >= tokens.size(); }

Token TokenBuffer::pop() { return tokens.at(index++); }

bool TokenBuffer::accept(TokenType token_type) const {
    return top().type == token_type;
}

Token TokenBuffer::expect(TokenType token_type) {
    auto token_or_empty = eat(token_type);
    if (!token_or_empty) {
        std::cout << reconstruct_row(top().row) << std::endl;
        util::print_col_indication(top().col);
        error::syntax(token_type, top());
        return top();
    }
    return token_or_empty.value();
}

bool TokenBuffer::can_pop(TokenType type, int offset) {
    if (index + offset >= tokens.size()) return false;
    return tokens.at(index + offset).type == type;
}

void TokenBuffer::backtrack(unsigned int steps) { index -= steps; }

std::string TokenBuffer::reconstruct_row(unsigned row) const {
    auto it = row_indices.find(row);
    if (it == std::end(row_indices)) {
        throw std::runtime_error("Unable to reconstruct row " +
                                 std::to_string(row));
    }

    unsigned current_index = it->second;

    std::ostringstream oss {};

    Token previous_token = tokens.at(current_index);

    oss << util::indent_spaces(previous_token.col - 1);
    oss << previous_token.value;

    while (1) {
        ++current_index;
        if (current_index >= tokens.size() ||
            tokens.at(current_index).row != row) {
            break;
        }

        Token current_token = tokens.at(current_index);

        unsigned spaces = current_token.col - previous_token.col -
                          previous_token.value.length();

        for (unsigned i = 0; i < spaces; ++i) {
            oss << " ";
        }

        oss << current_token.value;

        previous_token = current_token;
    }

    return oss.str();
}

TokenBuffer::Iterator TokenBuffer::top_iterator() {
    return tokens.begin() + index;
}

std::string TokenBuffer::as_source() const {
    if (tokens.empty()) return "";

    std::ostringstream oss {};

    Token previous = tokens.front();

    for (auto& token : tokens) {
        int row_dt = token.row - previous.row;

        if (row_dt) {
            for (int i = 0; i < row_dt; ++i) {
                oss << std::endl;
            }
            for (unsigned i = 0; i < token.col; ++i) {
                oss << " ";
            }
        } else {
            int spaces = std::max(
                0, (int)(token.col - previous.col - previous.value.length()));
            for (int i = 0; i < spaces; ++i) {
                oss << " ";
            }
        }

        oss << token.value;

        previous = token;
    }

    return oss.str();
}

TokenBuffer TokenBuffer::source_tokens(SourceRef source_ref) {
    TokenBuffer source_tokens {};
    while (source_ref.begin != source_ref.end) {
        source_tokens.add_token(*source_ref.begin);
        ++source_ref.begin;
    }

    return source_tokens;
}

void TokenBuffer::dump_current_row(const std::string_view error_message) const {
    auto reference_token = previous();
    std::cout << reconstruct_row(reference_token.row) << std::endl;
    util::print_col_indication(reference_token.col);
    if (error_message != "") {
        std::cout << error_message << std::endl;
    }
}
