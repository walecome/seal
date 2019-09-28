#include "Parser.hh"

ptr_t<While> Parser::parseWhile(TokenBuffer &tokens) {
    if (!tokens.eat(WHILE)) return nullptr;

    tokens.expect(LPARENS);
    ptr_t<Expression> condition = parseExpression(tokens);
    tokens.expect(RPARENS);
    ptr_t<Block> body = parseBlock(tokens);

    return std::make_unique<While>(condition, body);
}