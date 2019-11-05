#include "Parser.hh"

Type Parser::parse_type(TokenBuffer& tokens) {
    if (tokens.eat(TYPE)) {
        return Type { tokens.previous().value, Kind::PRIMITIVE };
    }

    tokens.expect(LBRACKET);
    tokens.expect(TYPE);
    auto type = Type { tokens.previous().value, Kind::ARRAY };
    tokens.expect(RBRACKET);

    return type;
}