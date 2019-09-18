#include "Parser.hh"

void Parser::parse(TokenBuffer &tokens) {
    while (parseFunction(tokens)) {
        ;
    };
}

ParseNode *Parser::parseFunction(TokenBuffer &tokens) {
    return parseFunctionDecl(tokens);
    // parseBlock(tokens);
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

    return new ParseNode {};
}

void Parser::parseBlock(TokenBuffer &tokens) {
    expect(tokens, LBRACE);

    expect(tokens, RBRACE);
}

// TODO: Should tokenbuffer expect instead?
void Parser::expect(TokenBuffer &tokens, TokenType tokenType) {
    if (!tokens.eat(tokenType)) throw std::runtime_error { "Invalid syntax" };
}