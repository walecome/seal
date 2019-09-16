#include "Parser.hh"

void Parser::parse(TokenBuffer &tokens) { parseFunction(tokens); }

void Parser::parseFunction(TokenBuffer &tokens) {
    parseFunctionDefinition(tokens);
}

void Parser::parseFunctionDefinition(TokenBuffer &tokens) {
    if (!tokens.eat(TokenType::FUNC_IDENT)) {
        throw std::runtime_error { "Error parsing function" };
    }

    expect(tokens, TokenType::FUNC_IDENT);
    expect(tokens, TokenType::IDENTIFIER);
    expect(tokens, TokenType::LPARENS);

    while (tokens.eat(TokenType::IDENTIFIER)) {
        expect(tokens, TokenType::COLON);
        expect(tokens, TokenType::TYPE);
    }

    expect(tokens, TokenType::RPARENS);
    expect(tokens, TokenType::ARROW);
    expect(tokens, TokenType::TYPE);
}

// TODO: Should tokenbuffer expect instead?
void Parser::expect(TokenBuffer &tokens, TokenType tokenType) {
    if (!tokens.eat(tokenType)) throw std::runtime_error { "Invalid syntax" };
}