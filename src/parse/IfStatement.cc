#include "ast/IfStatement.hh"
#include "Parser.hh"
#include "ast/Block.hh"
#include "ast/Expression.hh"

ptr_t<IfStatement> Parser::parse_if_statement(TokenBuffer &tokens) {
    auto begin = tokens.top_iterator();

    if (!tokens.eat(IF)) return nullptr;

    tokens.expect(LPARENS);

    ptr_t<Expression> condition = parse_expression(tokens);
    tokens.expect(RPARENS);

    ptr_t<Block> if_body = parse_block(tokens);

    ptr_t<Block> else_body = nullptr;

    if (tokens.eat(ELSE)) {
        else_body = parse_block(tokens);
    }

    auto end = tokens.top_iterator();

    auto if_statement =
        std::make_unique<IfStatement>(condition, if_body, else_body);

    if_statement->source_ref.begin = begin;
    if_statement->source_ref.end = end;

    return if_statement;
}