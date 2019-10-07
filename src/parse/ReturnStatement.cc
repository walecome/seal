#include "ast/ReturnStatement.hh"
#include "Parser.hh"
#include "ast/Expression.hh"

ptr_t<ReturnStatement> Parser::parse_return_statement(TokenBuffer &tokens) {
    if (!tokens.eat(RETURN)) return nullptr;

    ptr_t<Expression> return_value = parse_expression(tokens);
    tokens.expect(SEMICOLON);
    return std::make_unique<ReturnStatement>(return_value);
}