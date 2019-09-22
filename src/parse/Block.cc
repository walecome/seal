#include "Parser.hh"

ptr_t<Block> Parser::parseBlock(TokenBuffer &tokens) {
    if (!tokens.eat(LBRACE)) {
        return nullptr;
    }

    tokens.expect(RBRACE);

    std::cout << "Parsed block" << std::endl;
    return std::make_unique<Block>();
}
