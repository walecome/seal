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

    if (tokens.accept(IDENTIFIER)) {
        tokens.eat(IDENTIFIER);
        // TODO: Not sure if we should assume that an identifier is a struct.
        return { Primitive::USER_IDENTIFIER, Kind::STRUCT };
    }

    tokens.expect(TYPE);

    return { tokens.previous().value, Kind::PRIMITIVE };
}
