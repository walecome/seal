#include "ast/ReturnStatement.hh"
#include "Parser.hh"
#include "ast/Expression.hh"

ptr_t<ReturnStatement> Parser::parse_return_statement(TokenBuffer &tokens) {
    auto begin = tokens.top_iterator();

    if (!tokens.eat(RETURN)) return nullptr;

    ptr_t<Expression> return_value = parse_expression(tokens);
    tokens.expect(SEMICOLON);

    auto end = tokens.top_iterator();
    auto return_statement = std::make_unique<ReturnStatement>(return_value);

    return_statement->source_ref.begin = begin;
    return_statement->source_ref.end = end;

    return return_statement;
}