#include "Error.hh"

void error::syntax(TokenType expected, Token got) {
    std::ostringstream oss {};
    oss << "Invalid syntax, expected " << token_names[expected];
    oss << " got " << token_names[got.type];
    report_error(oss.str());
}

void error::syntax(const std::string &s, TokenBuffer &tokens) {
    std::ostringstream oss {};
    oss << s << " " << tokens.dump() << std::endl;
    report_error(oss.str());
}

void error::rpn(Token got) {
    std::ostringstream oss {};
    oss << "Invalid token when converting RPN to expression, "
        << " got " << token_names[got.type];
    report_error(oss.str());
}

void error::report_error(const std::string &error) {
    std::cout << Color::Modifier(Color::FG_RED) << "ERROR: " << error
              << Color::Modifier(Color::FG_DEFAULT) << std::endl;

    exit(EXIT_FAILURE);
}