#include "Parser.hh"

ptr_t<Function> Parser::parseFunction(TokenBuffer &tokens) {
    ptr_t<FunctionDecl> functionDeclNode = parseFunctionDecl(tokens);
    ptr_t<Block> blockNode = parseBlock(tokens);

    return std::make_unique<Function>(functionDeclNode, blockNode);
}

ptr_t<FunctionDecl> Parser::parseFunctionDecl(TokenBuffer &tokens) {
    if (!tokens.eat(FUNC_IDENT)) {
        return nullptr;
    }

    tokens.expect(IDENTIFIER);

    ptr_t<ParameterList> parameters = parseParameterList(tokens);

    tokens.expect(ARROW);
    tokens.expect(TYPE);

    return std::make_unique<FunctionDecl>(parameters);
}

ptr_t<ParameterList> Parser::parseParameterList(TokenBuffer &tokens) {
    tokens.expect(LPARENS);
    if (tokens.eat(RPARENS)) return nullptr;
    ptr_t<ParameterList> parameters = std::make_unique<ParameterList>();

    while (ptr_t<Parameter> parameter = parseParameter(tokens)) {
        parameters->addParamter(parameter);
        if (!tokens.eat(COMMA)) break;
    }

    tokens.expect(RPARENS);

    return parameters;
}

ptr_t<Parameter> Parser::parseParameter(TokenBuffer &tokens) {
    Token identifier = tokens.top();
    if (identifier.type != IDENTIFIER) return nullptr;
    tokens.eat(IDENTIFIER);

    tokens.expect(COLON);

    Token type = tokens.top();
    tokens.expect(TYPE);

    return std::make_unique<Parameter>(identifier.value,
                                       TypeUtil::fromString(type.value));
}
