#include "Parser.hh"

ptr_t<ArrayLiteral> Parser::parseArrayLiteral(TokenBuffer &tokens) {
    if (!tokens.eat(LBRACKET)) return nullptr;
    ptr_t<ArrayLiteral> array = std::make_unique<ArrayLiteral>();

    while (!tokens.eat(RBRACKET)) {
        ptr_t<Expression> expression = parseExpression(tokens);

        array->addExpression(expression);
    }

    return array;
}