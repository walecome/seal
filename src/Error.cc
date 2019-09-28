#include "Error.hh"

void Error::syntax(TokenType expected, Token got) {
    std::ostringstream oss {};
    oss << "Invalid syntax, expected " << tokenNames[expected];
    oss << " got " << tokenNames[got.type];
    reportError(oss.str());
}

void Error::syntax(const std::string &s, TokenBuffer &tokens) {
    std::ostringstream oss {};
    oss << s << " " << tokens.dump() << std::endl;
    reportError(oss.str());
}

void Error::rpn(Token got) {
    std::ostringstream oss {};
    oss << "Invalid token when converting RPN to expression, "
        << " got " << tokenNames[got.type];
    reportError(oss.str());
}

void Error::reportError(const std::string &error) {
    std::cout << Color::Modifier(Color::FG_RED) << "ERROR: " << error
              << Color::Modifier(Color::FG_DEFAULT) << std::endl;

    exit(EXIT_FAILURE);
}