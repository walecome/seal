#include "ast/While.hh"
#include "Block.hh"
#include "Parser.hh"
#include "ast/Expression.hh"

ptr_t<While> Parser::parse_while(TokenBuffer &tokens) {
    auto begin = tokens.top_iterator();

    if (!tokens.eat(WHILE)) return nullptr;

    tokens.expect(LPARENS);
    ptr_t<Expression> condition = parse_expression(tokens);
    tokens.expect(RPARENS);
    ptr_t<Block> body = parse_block(tokens);

    auto end = tokens.top_iterator();
    auto _while = std::make_unique<While>(condition, body);

    _while->source_ref.begin = begin;
    _while->source_ref.end = end;

    return _while;
}