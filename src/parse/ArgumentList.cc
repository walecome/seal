#include "Parser.hh"

ptr_t<ArgumentList> Parser::parseArgumentList(TokenBuffer &tokens) {
    tokens.expect(LPARENS);

    ptr_t<ArgumentList> argumentList = std::make_unique<ArgumentList>();
    if (tokens.eat(RPARENS)) return argumentList;

    do {
        ptr_t<Expression> argumentExpression = Parser::parseExpression(tokens);
        argumentList->addArgument(argumentExpression);
    } while (!tokens.eat(RPARENS));

    return argumentList;
}
