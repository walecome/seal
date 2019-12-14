#include "Parser.hh"

Type Parser::parse_type(TokenBuffer& tokens) {
    // Try parsing the array type before the other as it will give better error
    // messages
    if (tokens.eat(LBRACKET)) {
        tokens.expect(TYPE);
        auto type = Type { tokens.previous().value, Kind::ARRAY };
        tokens.expect(RBRACKET);

        return type;
    }

    if (tokens.top().type == ASSIGN) {
        return { Primitive::INFERRED };
    }

    tokens.expect(TYPE);

    return { tokens.previous().value, Kind::PRIMITIVE };
}
