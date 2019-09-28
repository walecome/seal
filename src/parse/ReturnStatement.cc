#include "Parser.hh"

ptr_t<ReturnStatement> Parser::parseReturnStatement(TokenBuffer &tokens) {
    if (!tokens.eat(RETURN)) return nullptr;

    ptr_t<Expression> returnValue = parseExpression(tokens);

    return std::make_unique<ReturnStatement>(returnValue);
}