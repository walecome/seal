#pragma once

#include <stdexcept>

#include "Token.hh"
#include "TokenBuffer.hh"

namespace Color {
enum Code {
    FG_RED = 31,
    FG_GREEN = 32,
    FG_BLUE = 34,
    FG_DEFAULT = 39,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_BLUE = 44,
    BG_DEFAULT = 49
};
class Modifier {
    Code code;

   public:
    Modifier(Code p_code) : code(p_code) {}
    friend std::ostream& operator<<(std::ostream& os, const Modifier& mod) {
        return os << "\033[" << mod.code << "m";
    }
};
}  // namespace Color

namespace error {
void syntax(TokenType expected, Token got);
void syntax(const std::string& s, TokenBuffer& tokens);
void report_error(const std::string& error);
void rpn(Token got);
}  // namespace error