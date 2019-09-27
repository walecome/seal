#include "Error.hh"

void Error::syntax(TokenType expected, Token got) {
    std::ostringstream os {};
    os << "Invalid syntax, expected " << tokenNames[expected];
    os << " got " << tokenNames[got.type];
    throw std::runtime_error { os.str() };
}

void Error::rpn(Token got) {
    std::ostringstream os {};
    os << "Invalid token when converting RPN to expression, "
       << " got " << tokenNames[got.type];
    throw std::runtime_error { os.str() };
}