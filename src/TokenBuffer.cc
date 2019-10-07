#include "TokenBuffer.hh"
#include "Error.hh"

void TokenBuffer::add_token(const Token token) { tokens.push_back(token); }

void TokenBuffer::print_tokens() const { std::cout << dump() << std::endl; }

std::string TokenBuffer::dump() const {
    std::ostringstream oss {};
    oss << "Tokens (" << tokens.size() << "):" << std::endl;
    for (auto token : tokens) {
        oss << "\t" << token.to_string() << std::endl;
    }

    return oss.str();
}

bool TokenBuffer::eat(const TokenType token_type) {
    if (tokens.at(index).type == token_type) {
        ++index;
        return true;
    }

    return false;
}

bool TokenBuffer::empty() const { return index >= tokens.size(); }

Token TokenBuffer::pop() { return tokens.at(index++); }

bool TokenBuffer::accept(TokenType token_type) const {
    return top().type == token_type;
}

void TokenBuffer::expect(TokenType token_type) {
    if (top().type != token_type) {
        error::syntax(token_type, top());
    }
    eat(token_type);
}

bool TokenBuffer::can_pop(TokenType type, int offset) {
    if (index + offset >= tokens.size()) return false;
    return tokens.at(index + offset).type == type;
}

void TokenBuffer::backtrack(unsigned int steps) { index -= steps; }