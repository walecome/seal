#include "ast/VariableDecl.hh"
#include <optional>
#include "Parser.hh"
#include "ast/Expression.hh"

ptr_t<VariableDecl> Parser::parse_variable_decl(TokenBuffer& tokens) {
    auto begin = tokens.top_iterator();

    if (!tokens.can_pop(IDENTIFIER) || !tokens.can_pop(COLON, 1))
        return nullptr;

    // @REFACTOR: The API for can_pop can be improved, we shouldn't need to eat
    // the tokens after checking
    Token identifier = tokens.top();
    tokens.expect(IDENTIFIER);
    tokens.expect(COLON);

    bool is_mutable = tokens.eat(MUTABLE);

    Type type = parse_type(tokens);
    tokens.expect(ASSIGN);

    ptr_t<Expression> value = parse_expression(tokens);

    tokens.expect(SEMICOLON);

    auto end = tokens.top_iterator();

    auto var_decl =
        std::make_unique<VariableDecl>(identifier, type, value, is_mutable);

    var_decl->source_ref.begin = begin;
    var_decl->source_ref.end = end;

    return var_decl;
}