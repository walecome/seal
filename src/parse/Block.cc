#include "ast/Block.hh"
#include "Parser.hh"

ptr_t<Block> Parser::parse_block(TokenBuffer &tokens) {
    auto begin = tokens.top_iterator();

    if (!tokens.eat(LBRACE)) {
        return nullptr;
    }

    ptr_t<Block> block = std::make_unique<Block>();

    while (!tokens.eat(RBRACE)) {
        if (ptr_t<Node> function = parse_function_decl(tokens)) {
            block->add_node(function);
        } else {
            ptr_t<Node> statement = parse_statement(tokens);
            block->add_node(statement);
        }
    }

    auto end = tokens.top_iterator();

    block->source_ref.begin = begin;
    block->source_ref.end = end;

    return block;
}
