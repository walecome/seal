#include "ast/IfStatement.hh"
#include "Parser.hh"
#include "ast/Block.hh"
#include "ast/Expression.hh"

ptr_t<IfStatement> Parser::parseIfStatement(TokenBuffer &tokens) {
    if (!tokens.eat(IF)) return nullptr;

    tokens.expect(LPARENS);

    ptr_t<Expression> condition = parseExpression(tokens);
    tokens.expect(RPARENS);

    ptr_t<Block> ifBody = parseBlock(tokens);

    ptr_t<Block> elseBody = nullptr;

    if (tokens.eat(ELSE)) {
        elseBody = parseBlock(tokens);
    }

    return std::make_unique<IfStatement>(condition, ifBody, elseBody);
}