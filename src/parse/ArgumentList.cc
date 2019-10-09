#include "ast/ArgumentList.hh"
#include "Parser.hh"

ptr_t<ArgumentList> Parser::parse_argument_list(TokenBuffer &tokens) {
    auto begin = tokens.top_iterator();

    tokens.expect(LPARENS);

    ptr_t<ArgumentList> argument_list = std::make_unique<ArgumentList>();

    if (tokens.eat(RPARENS)) return argument_list;

    do {
        ptr_t<Expression> argument_expression =
            Parser::parse_expression(tokens);
        argument_list->add_argument(argument_expression);
    } while (tokens.eat(COMMA));

    tokens.expect(RPARENS);

    auto end = tokens.top_iterator();

    argument_list->source_ref.begin = begin;
    argument_list->source_ref.end = end;

    return argument_list;
}
