#include "Block.hh"

void Block::addStatement(ptr_t<Statement> &statement) {
    statements.push_back(std::move(statement));
}