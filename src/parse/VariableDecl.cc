#include "ast/VariableDecl.hh"
#include "Parser.hh"
#include "ast/Expression.hh"

ptr_t<VariableDecl> Parser::parse_variable_decl(TokenBuffer &tokens) {
    if (!tokens.can_pop(IDENTIFIER) || !tokens.can_pop(COLON, 1))
        return nullptr;

    // @REFACTOR: The API for can_pop can be improved, we shouldn't need to eat
    // the tokens after checking
    Token identifier = tokens.top();
    tokens.expect(IDENTIFIER);
    tokens.expect(COLON);

    Token type = tokens.top();
    tokens.expect(TYPE);
    tokens.expect(ASSIGN);

    ptr_t<Expression> value = parse_expression(tokens);

    tokens.expect(SEMICOLON);

    return std::make_unique<VariableDecl>(identifier, type.value, value);
}