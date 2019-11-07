#include "ast/Statement.hh"
#include "Parser.hh"
#include "ast/Block.hh"
#include "ast/Expression.hh"
#include "ast/For.hh"
#include "ast/IfStatement.hh"
#include "ast/ReturnStatement.hh"
#include "ast/VariableDecl.hh"
#include "ast/While.hh"

ptr_t<Statement> Parser::parse_statement(TokenBuffer &tokens) {
    ptr_t<Statement> statement;
    if ((statement = parse_variable_decl(tokens))) return statement;
    if ((statement = parse_return_statement(tokens))) return statement;
    if ((statement = parse_while(tokens))) return statement;
    if ((statement = parse_block(tokens))) return statement;
    if ((statement = parse_if_statement(tokens))) return statement;
    if ((statement = parse_for(tokens))) return statement;

    statement = parse_expression(tokens);
    tokens.expect(SEMICOLON);
    return statement;
}