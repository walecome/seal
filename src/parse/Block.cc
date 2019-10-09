#include "ast/Block.hh"
#include "Parser.hh"

ptr_t<Block> Parser::parse_block(TokenBuffer &tokens) {
    auto begin = tokens.top_iterator();

    if (!tokens.eat(LBRACE)) {
        return nullptr;
    }

    ptr_t<Block> block = std::make_unique<Block>();

    while (!tokens.eat(RBRACE)) {
        ptr_t<Statement> statement = parse_statement(tokens);
        block->add_statement(statement);
    }

    auto end = tokens.top_iterator();

    block->source_ref.begin = begin;
    block->source_ref.end = std::next(end);

    return block;
}
