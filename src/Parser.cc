#include "Parser.hh"

void Parser::parse(TokenBuffer &tokens) {
    while (!tokens.empty()) {
        std::cout << "Parsing function" << std::endl;
        ptr_t<Function> function = parseFunction(tokens);

        if (!function) break;

        parseNodes.push_back(std::move(function));
    };
}

ptr_t<Function> Parser::parseFunction(TokenBuffer &tokens) {
    ptr_t<FunctionDecl> functionDeclNode = parseFunctionDecl(tokens);
    ptr_t<Block> blockNode = parseBlock(tokens);

    return std::make_unique<Function>(functionDeclNode, blockNode);
}

ptr_t<FunctionDecl> Parser::parseFunctionDecl(TokenBuffer &tokens) {
    if (!tokens.eat(FUNC_IDENT)) {
        return nullptr;
    }

    expect(tokens, IDENTIFIER);

    ptr_t<ParameterList> parameters = parseParameterList(tokens);

    expect(tokens, ARROW);
    expect(tokens, TYPE);

    std::cout << "Parsed function declaration" << std::endl;
    return std::make_unique<FunctionDecl>(parameters);
}

ptr_t<ParameterList> Parser::parseParameterList(TokenBuffer &tokens) {
    expect(tokens, LPARENS);
    if (tokens.eat(RPARENS)) return nullptr;
    ptr_t<ParameterList> parameters = std::make_unique<ParameterList>();

    while (ptr_t<Parameter> parameter = parseParameter(tokens)) {
        parameters->addParamter(parameter);
        // @TODO: This is kinda ugly, we need to eat the comma to process the
        //        next token.
        tokens.eat(COMMA);
    }

    expect(tokens, RPARENS);

    return parameters;
}

ptr_t<Parameter> Parser::parseParameter(TokenBuffer &tokens) {
    Token identifier = tokens.top();
    if (identifier.type != IDENTIFIER) return nullptr;
    tokens.eat(IDENTIFIER);

    expect(tokens, COLON);

    Token type = tokens.top();
    expect(tokens, TYPE);

    return std::make_unique<Parameter>(identifier.value,
                                       TypeUtil::fromString(type.value));
}

ptr_t<Block> Parser::parseBlock(TokenBuffer &tokens) {
    if (!tokens.eat(LBRACE)) {
        return nullptr;
    }

    expect(tokens, RBRACE);

    std::cout << "Parsed block" << std::endl;
    return std::make_unique<Block>();
}

// TODO: Should tokenbuffer expect instead?
void Parser::expect(TokenBuffer &tokens, TokenType tokenType) {
    std::ostringstream os {};
    os << "Invalid syntax, expected " << tokenNames[tokenType];
    if (tokens.top().type != tokenType) {
        os << " got " << tokenNames[tokens.top().type];
        throw std::runtime_error { os.str() };
    }
    tokens.eat(tokenType);
}