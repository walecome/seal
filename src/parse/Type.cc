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
    tokens.expect(TYPE);
    return Type { tokens.previous().value, Kind::PRIMITIVE };
}
