#include "Token.hh"

TokenType stringToToken(const std::string &s) {
    auto it = stringTokenMap.find(s);

    if (it == std::end(stringTokenMap)) {
        return TokenType::UNEXPECTED;
    }

    return it->second;
}

void Token::print() const { std::cout << value << std::endl; }