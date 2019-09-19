#include "Parser.hh"

void Parser::parse(TokenBuffer &tokens) {
    while (!tokens.empty() && parseFunction(tokens)) {
        std::cout << "Parsing function" << std::endl;
    };
}

ParseNode *Parser::parseFunction(TokenBuffer &tokens) {
    ParseNode *functionDeclNode = parseFunctionDecl(tokens);
    ParseNode *blockNode = parseBlock(tokens);
    return functionDeclNode;
}

ParseNode *Parser::parseFunctionDecl(TokenBuffer &tokens) {
    if (!tokens.eat(FUNC_IDENT)) {
        return nullptr;
    }
    expect(tokens, IDENTIFIER);
    expect(tokens, LPARENS);

    while (tokens.eat(IDENTIFIER)) {
        expect(tokens, COLON);
        expect(tokens, TYPE);
    }

    expect(tokens, RPARENS);
    expect(tokens, ARROW);
    expect(tokens, TYPE);

    std::cout << "Parsed function declaration" << std::endl;
    return new ParseNode {};
}

ParseNode *Parser::parseBlock(TokenBuffer &tokens) {
    if (!tokens.eat(LBRACE)) {
        return nullptr;
    }

    if (tokens.accept(LBRACE)) parseBlock(tokens);

    expect(tokens, RBRACE);

    std::cout << "Parsed block" << std::endl;
    return new ParseNode {};
}

// TODO: Should tokenbuffer expect instead?
void Parser::expect(TokenBuffer &tokens, TokenType tokenType) {
    std::ostringstream os {};
    os << "Invalid syntax, expected " << tokenNames[tokenType] << std::endl;
    if (!tokens.eat(tokenType)) throw std::runtime_error { os.str() };
}