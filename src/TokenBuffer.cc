#include "TokenBuffer.hh"

void TokenBuffer::addToken(const Token token) { tokens.push_back(token); }

void TokenBuffer::printTokens() const {
    std::cout << "Tokens (" << tokens.size() << "):" << std::endl;
    for (auto token : tokens) {
        std::cout << "\t" << token.toString() << std::endl;
    }
}

bool TokenBuffer::eat(const TokenType tokenType) {
    if (tokens.at(index).type == tokenType) {
        ++index;
        return true;
    }

    return false;
}

bool TokenBuffer::empty() const { return index >= tokens.size(); }

Token TokenBuffer::pop() { return tokens.at(index++); }

bool TokenBuffer::accept(TokenType tokenType) const {
    return top().type == tokenType;
}

void TokenBuffer::expect(TokenType tokenType) {
    std::ostringstream os {};
    os << "Invalid syntax, expected " << tokenNames[tokenType];
    if (top().type != tokenType) {
        os << " got " << tokenNames[top().type];
        throw std::runtime_error { os.str() };
    }
    eat(tokenType);
}