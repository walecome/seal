#include "ast/Function.hh"
#include "Parser.hh"
#include "ast/FunctionDecl.hh"
#include "ast/ParameterList.hh"
#include "ast/VariableDecl.hh"

ptr_t<Function> Parser::parseFunction(TokenBuffer &tokens) {
    ptr_t<FunctionDecl> functionDeclNode = parseFunctionDecl(tokens);
    if (!functionDeclNode) return nullptr;

    ptr_t<Block> blockNode = parseBlock(tokens);
    if (!blockNode) return nullptr;

    return std::make_unique<Function>(functionDeclNode, blockNode);
}

ptr_t<FunctionDecl> Parser::parseFunctionDecl(TokenBuffer &tokens) {
    if (!tokens.eat(FUNC_KEYWORD)) {
        return nullptr;
    }

    Token identifier = tokens.top();
    tokens.expect(IDENTIFIER);

    ptr_t<ParameterList> parameters = parseParameterList(tokens);

    tokens.expect(ARROW);
    tokens.expect(TYPE);

    return std::make_unique<FunctionDecl>(identifier.value, parameters);
}

ptr_t<ParameterList> Parser::parseParameterList(TokenBuffer &tokens) {
    tokens.expect(LPARENS);
    if (tokens.eat(RPARENS)) return nullptr;
    ptr_t<ParameterList> parameters = std::make_unique<ParameterList>();

    while (ptr_t<VariableDecl> parameter = parseParameter(tokens)) {
        parameters->addParameter(parameter);
        if (!tokens.eat(COMMA)) break;
    }

    tokens.expect(RPARENS);

    return parameters;
}

ptr_t<VariableDecl> Parser::parseParameter(TokenBuffer &tokens) {
    Token identifier = tokens.top();
    if (identifier.type != IDENTIFIER) return nullptr;
    tokens.eat(IDENTIFIER);

    tokens.expect(COLON);

    Token type = tokens.top();
    tokens.expect(TYPE);

    return std::make_unique<VariableDecl>(identifier.value, type.value);
}
