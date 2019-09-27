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
    if (top().type != tokenType) {
        Error::syntax(tokenType, top());
    }
    eat(tokenType);
}