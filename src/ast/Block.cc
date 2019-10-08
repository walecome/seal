#include "Block.hh"

void Block::add_statement(ptr_t<Statement> &statement) {
    statements.push_back(std::move(statement));
}

void Block::analyze(Scope *scope) {
    for (auto &statement : statements) {
        statement->analyze(scope);
    }
}

std::string Block::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " [" << std::endl;

    for (auto &x : statements) {
        oss << x->dump(indent + 1) << std::endl;
    }

    oss << util::indent(indent) << "]";
    return oss.str();
}