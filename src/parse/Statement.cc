

#include "Parser.hh"

ptr_t<Statement> Parser::parseStatement(TokenBuffer &tokens) {
    ptr_t<Statement> statement;
    if ((statement = parseVariableDecl(tokens))) return statement;

    statement = parseExpression(tokens);
    return statement;
}