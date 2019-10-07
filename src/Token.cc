#include "Token.hh"

TokenType string_to_token(const std::string_view s) {
    auto it = string_token_map.find(s);

    if (it == std::end(string_token_map)) {
        return UNEXPECTED;
    }

    return it->second;
}

std::string Token::to_string() const {
    std::ostringstream os {};

    auto it = token_names.find(type);
    if (it == std::end(token_names))
        throw std::runtime_error("Unable to find token type in token names");

    os << "{ " << it->second << ", " << value << ", " << row << ", " << col
       << "}";

    return os.str();
}

bool is_type(const std::string_view s) {
    return TypeUtil::string_to_type.find(s) !=
           std::end(TypeUtil::string_to_type);
}

void try_replace_keyword_or_type(Token& token) {
    auto it = keyword_map.find(token.value);
    if (it == std::end(string_token_map)) {
        if (is_type(token.value)) {
            token.type = TYPE;
        }

        return;
    }

    token.type = it->second;
}