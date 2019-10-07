#include "ast/While.hh"
#include "Block.hh"
#include "Parser.hh"
#include "ast/Expression.hh"

ptr_t<While> Parser::parse_while(TokenBuffer &tokens) {
    if (!tokens.eat(WHILE)) return nullptr;

    tokens.expect(LPARENS);
    ptr_t<Expression> condition = parse_expression(tokens);
    tokens.expect(RPARENS);
    ptr_t<Block> body = parse_block(tokens);

    return std::make_unique<While>(condition, body);
}