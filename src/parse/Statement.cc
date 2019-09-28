#include "Parser.hh"

ptr_t<Statement> Parser::parseStatement(TokenBuffer &tokens) {
    ptr_t<Statement> statement;
    if ((statement = parseVariableDecl(tokens))) return statement;
    if ((statement = parseReturnStatement(tokens))) return statement;
    if ((statement = parseWhile(tokens))) return statement;
    if ((statement = parseBlock(tokens))) return statement;
    if ((statement = parseIfStatement(tokens))) return statement;

    statement = parseExpression(tokens);
    return statement;
}