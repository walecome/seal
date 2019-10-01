#include "ast/Statement.hh"
#include "Parser.hh"
#include "ast/Block.hh"
#include "ast/Expression.hh"
#include "ast/IfStatement.hh"
#include "ast/ReturnStatement.hh"
#include "ast/VariableDecl.hh"
#include "ast/While.hh"

ptr_t<Statement> Parser::parseStatement(TokenBuffer &tokens) {
    ptr_t<Statement> statement;
    if ((statement = parseVariableDecl(tokens))) return statement;
    if ((statement = parseReturnStatement(tokens))) return statement;
    if ((statement = parseWhile(tokens))) return statement;
    if ((statement = parseBlock(tokens))) return statement;
    if ((statement = parseIfStatement(tokens))) return statement;

    statement = parseExpression(tokens);
    tokens.expect(SEMICOLON);
    return statement;
}