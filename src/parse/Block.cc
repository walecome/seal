#include "ast/Block.hh"
#include "Parser.hh"

ptr_t<Block> Parser::parseBlock(TokenBuffer &tokens) {
    if (!tokens.eat(LBRACE)) {
        return nullptr;
    }

    ptr_t<Block> block = std::make_unique<Block>();

    while (!tokens.eat(RBRACE)) {
        ptr_t<Statement> statement = parseStatement(tokens);
        if (verbose)
            std::cout << "Parsed statement: " << statement->dump() << std::endl;
        block->addStatement(statement);
    }

    return block;
}
