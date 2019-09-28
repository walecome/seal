
#include "Parser.hh"

ptr_t<VariableDecl> Parser::parseVariableDecl(TokenBuffer &tokens) {
    if (!tokens.canPop(IDENTIFIER) || !tokens.canPop(COLON, 1)) return nullptr;

    // @REFACTOR: The API for canPop can be improved, we shouldn't need to eat
    // the tokens after checking
    Token identifier = tokens.top();
    tokens.expect(IDENTIFIER);
    tokens.expect(COLON);

    Token type = tokens.top();
    tokens.expect(TYPE);
    tokens.expect(ASSIGN);

    ptr_t<Expression> value = parseExpression(tokens);

    return std::make_unique<VariableDecl>(identifier.value, type.value, value);
}