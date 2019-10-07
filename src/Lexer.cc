#include "Lexer.hh"

bool is_string(const char c) { return c == '"' || c == '\''; }

void Lexer::read_all() {
    read_whitespace();
    while (current_index < source.size()) {
        Token token = read_one();
        tokens.add_token(token);
        read_whitespace();
    }
}

Token Lexer::read_one() {
    Token token { row, col };

    const char c = source[current_index];

    if (std::isalpha(c)) {
        read_alpha(token);
    } else if (std::isdigit(c)) {
        read_number(token);
    } else if (is_string(c)) {
        read_string(token, c);
    } else {
        read_symbol(token);
    }

    col += token.value.length();

    return token;
}

void Lexer::read_whitespace() {
    char c = source[current_index];
    while (isspace(c)) {
        if (c == '\n') {
            ++row;
            col = 0;
        } else {
            ++col;
        }

        c = source[++current_index];
    }
}

void Lexer::read_alpha(Token &token) {
    token.type = IDENTIFIER;
    unsigned start_index = current_index;

    char current_char = source[current_index];
    while (std::isalpha(current_char) ||
           (start_index != current_index &&
            (std::isdigit(current_char) || current_char == '_'))) {
        current_char = source[++current_index];
    }

    token.value = cut(start_index, current_index);

    try_replace_keyword_or_type(token);
}

void Lexer::read_number(Token &token) {
    token.type = NUMBER;
    unsigned start_index = current_index;

    char current_char = source[current_index];
    while (isdigit(current_char)) {
        current_char = source[++current_index];
    }

    token.value = cut(start_index, current_index);
}

void Lexer::read_string(Token &token, char string_opener) {
    token.type = STRING;
    unsigned start_index = current_index;

    // Start of string
    char current_char = source[++current_index];

    while (current_char != string_opener) {
        if (current_char == '\\') ++current_index;
        current_char = source[++current_index];
    }

    token.value = cut(start_index, ++current_index);
}

void Lexer::read_symbol(Token &token) {
    TokenType type;
    std::string_view value;
    std::tie(type, value) = find_longest_matching_token();

    token.type = type;
    token.value = value;
}

inline std::pair<TokenType, std::string_view>
Lexer::find_longest_matching_token() {
    unsigned start_index = current_index;

    TokenType longest_symbol { UNEXPECTED };
    std::string_view longest { "" };

    while (1) {
        ++current_index;
        if (current_index > source.size()) break;

        std::string_view current_string = cut(start_index, current_index);

        TokenType found_symbol = string_to_token(current_string);
        if (found_symbol == UNEXPECTED) {
            break;
        }

        longest_symbol = found_symbol;
        longest = current_string;
    }
    --current_index;

    if (longest_symbol == UNEXPECTED) {
        report_error(cut(start_index, current_index + 1));
    };

    return { longest_symbol, longest };
}

std::string_view Lexer::cut(unsigned start, unsigned end) const {
    return source.substr(start, end - start);
}

void Lexer::report_error(const std::string_view value) const {
    std::cout << "Unable to lex token: " << value << std::endl;
    exit(EXIT_FAILURE);
}