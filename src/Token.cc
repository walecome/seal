#include "Token.hh"

TokenType stringToToken(const std::string &s) {
    auto it = stringTokenMap.find(s);

    if (it == std::end(stringTokenMap)) {
        return TokenType::UNEXPECTED;
    }

    return it->second;
}

void Token::print() const { std::cout << value << std::endl; }

bool isType(const std::string &s) { return types.find(s) != std::end(types); }

void tryReplaceKeywordOrType(Token &token) {
    auto it = keywordMap.find(token.value);
    if (it == std::end(stringTokenMap)) {
        if (isType(token.value)) {
            token.type = TokenType::TYPE;
        }

        return;
    }

    token.type = it->second;
}