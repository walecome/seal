#include "Token.hh"

TokenType stringToToken(const std::string &s) {
    auto it = stringTokenMap.find(s);

    if (it == std::end(stringTokenMap)) {
        return UNEXPECTED;
    }

    return it->second;
}

std::string Token::toString() const {
    std::ostringstream os {};

    auto it = tokenNames.find(type);
    if (it == std::end(tokenNames))
        throw std::runtime_error("Unable to find token type in token names");

    os << "{ " << it->second << ", " << value << " }";

    return os.str();
}

bool isType(const std::string &s) {
    return TypeUtil::stringToType.find(s) != std::end(TypeUtil::stringToType);
}

void tryReplaceKeywordOrType(Token &token) {
    auto it = keywordMap.find(token.value);
    if (it == std::end(stringTokenMap)) {
        if (isType(token.value)) {
            token.type = TYPE;
        }

        return;
    }

    token.type = it->second;
}