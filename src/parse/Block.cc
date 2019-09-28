#include "Parser.hh"

ptr_t<Block> Parser::parseBlock(TokenBuffer &tokens) {
    if (!tokens.eat(LBRACE)) {
        return nullptr;
    }

    while (!tokens.eat(RBRACE)) {
        ptr_t<Statement> statement = parseStatement(tokens);
        std::cout << "Parsed statement: " << statement->dump() << std::endl;
    }

    return std::make_unique<Block>();
}
