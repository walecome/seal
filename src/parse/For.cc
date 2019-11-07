#include "ast/For.hh"
#include "Parser.hh"

ptr_t<For> Parser::parse_for(TokenBuffer &tokens) {
    auto begin = tokens.top_iterator();
    if (!tokens.eat(FOR)) return nullptr;

    tokens.expect(LPARENS);
    auto initial_decl = parse_variable_decl(tokens);
    // @TODO: Give a good error
    ASSERT(initial_decl.get());
    // parse_variable_decl() eats the semicolon
    auto stop_condition = parse_expression(tokens);
    tokens.expect(SEMICOLON);
    auto iteration_expression = parse_expression(tokens);
    tokens.expect(RPARENS);
    auto body = parse_block(tokens);

    auto end = tokens.top_iterator();

    auto for_statement = std::make_unique<For>(initial_decl, stop_condition,
                                               iteration_expression, body);

    for_statement->source_ref.begin = begin;
    for_statement->source_ref.end = end;

    return for_statement;
}