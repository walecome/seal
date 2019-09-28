#include "Parser.hh"

ptr_t<Block> Parser::parseBlock(TokenBuffer &tokens) {
    if (!tokens.eat(LBRACE)) {
        return nullptr;
    }

    int a = 0;
    while (!tokens.eat(RBRACE)) {
        std::cout << a++ << std::endl;
        std::cout << tokens.top().toString() << std::endl;

        parseExpression(tokens);
    }

    return std::make_unique<Block>();
}
