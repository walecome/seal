#include "Parser.hh"

Parser::~Parser() {
    for (auto x : parseNodes) {
        delete x;
    }
}

void Parser::parse(TokenBuffer &tokens) {
    while (!tokens.empty()) {
        std::cout << "Parsing function" << std::endl;
        Function *function = parseFunction(tokens);

        if (!function) break;

        parseNodes.push_back(function);
    };
}

Function *Parser::parseFunction(TokenBuffer &tokens) {
    FunctionDecl *functionDeclNode = parseFunctionDecl(tokens);
    Block *blockNode = parseBlock(tokens);
    return new Function { functionDeclNode, blockNode };
}

FunctionDecl *Parser::parseFunctionDecl(TokenBuffer &tokens) {
    if (!tokens.eat(FUNC_IDENT)) {
        return nullptr;
    }

    expect(tokens, IDENTIFIER);

    ParameterList *parameters = parseParameterList(tokens);

    expect(tokens, ARROW);
    expect(tokens, TYPE);

    std::cout << "Parsed function declaration" << std::endl;
    return new FunctionDecl { parameters };
}

ParameterList *Parser::parseParameterList(TokenBuffer &tokens) {
    expect(tokens, LPARENS);
    if (tokens.eat(RPARENS)) return nullptr;
    ParameterList *parameters { new ParameterList {} };

    while (Parameter *parameter = parseParameter(tokens)) {
        parameters->addParamter(parameter);
        // @TODO: This is kinda ugly, we need to eat the comma to process the
        //        next token.
        tokens.eat(COMMA);
    }

    expect(tokens, RPARENS);

    return parameters;
}

Parameter *Parser::parseParameter(TokenBuffer &tokens) {
    Token identifier = tokens.top();
    if (identifier.type != IDENTIFIER) return nullptr;
    tokens.eat(IDENTIFIER);

    expect(tokens, COLON);

    Token type = tokens.top();
    expect(tokens, TYPE);

    return new Parameter { identifier.value, TypeUtil::fromString(type.value) };
}

Block *Parser::parseBlock(TokenBuffer &tokens) {
    if (!tokens.eat(LBRACE)) {
        return nullptr;
    }

    expect(tokens, RBRACE);

    std::cout << "Parsed block" << std::endl;
    return new Block {};
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