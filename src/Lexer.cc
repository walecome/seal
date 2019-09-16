#include "Lexer.hh"

bool isString(const char c) { return c == '"'; }

void Lexer::readAll() {
    readWhitespace();
    while (current_index < source.size()) {
        Token token = readOne();
        tokens.push_back(token);
        readWhitespace();
    }
}

Token Lexer::readOne() {
    Token token {};

    const char c = source[current_index];

    if (std::isalpha(c)) {
        readAlpha(token);
    } else if (std::isdigit(c)) {
        readNumber(token);
    } else if (isString(c)) {
        readString(token);
    } else {
        readSymbol(token);
    }

    return token;
}

void Lexer::readWhitespace() {
    char c = source[current_index];
    while (isspace(c)) {
        c = source[++current_index];
    }
}

void Lexer::readAlpha(Token &token) {
    token.type = TokenType::IDENTIFIER;
    unsigned start_index = current_index;

    char current_char = source[current_index];
    while (std::isalpha(current_char) ||
           (start_index != current_index &&
            (std::isdigit(current_char) || current_char == '_'))) {
        current_char = source[++current_index];
    }

    token.value = cut(start_index, current_index);
}

void Lexer::readNumber(Token &token) {
    token.type = TokenType::NUMBER;
    unsigned start_index = current_index;

    char current_char = source[current_index];
    while (isdigit(current_char)) {
        current_char = source[++current_index];
    }

    token.value = cut(start_index, current_index);
}

void Lexer::readString(Token &token) {
    token.type = TokenType::STRING;
    unsigned start_index = current_index;

    // Start of string
    char current_char = source[++current_index];

    while (current_char != '"') {
        ++current_index;
    }

    token.value = cut(start_index, current_index);
}

void Lexer::readSymbol(Token &token) {
    TokenType type;
    std::string value;
    std::tie(type, value) = findLongestMatchingToken();

    token.type = type;
    token.value = value;
}

inline std::pair<TokenType, std::string> Lexer::findLongestMatchingToken() {
    unsigned start_index = current_index;

    TokenType longest_symbol { TokenType::UNEXPECTED };
    std::string longest = "";

    while (1) {
        ++current_index;
        if (current_index > source.size()) break;

        std::string current_string = cut(start_index, current_index);

        TokenType found_symbol = stringToToken(current_string);
        if (found_symbol == TokenType::UNEXPECTED) {
            break;
        }

        longest_symbol = found_symbol;
        longest = current_string;
    }
    --current_index;

    if (longest_symbol == TokenType::UNEXPECTED) throw 1;

    return { longest_symbol, longest };
}

void Lexer::printTokens() const {
    for (auto token : tokens) {
        token.print();
    }
}

std::string Lexer::cut(unsigned start, unsigned end) const {
    return source.substr(start, end - start);
}