#pragma once

#include <stdexcept>

#include "Token.hh"

namespace Error {
void syntax(TokenType expected, Token got);

void rpn(Token got);
}  // namespace Error