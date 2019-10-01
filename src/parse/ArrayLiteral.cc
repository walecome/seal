#include "ast/ArrayLiteral.hh"
#include "Parser.hh"
#include "ast/Expression.hh"

ptr_t<ArrayLiteral> Parser::parseArrayLiteral(TokenBuffer &tokens) {
    if (!tokens.eat(LBRACKET)) return nullptr;
    ptr_t<ArrayLiteral> array = std::make_unique<ArrayLiteral>();

    do {
        ptr_t<Expression> expression = Parser::parseExpression(tokens);
        array->addExpression(expression);
    } while (tokens.eat(COMMA));

    tokens.expect(RBRACKET);

    return array;
}