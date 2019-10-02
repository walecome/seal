#include "Block.hh"

void Block::addStatement(ptr_t<Statement> &statement) {
    statements.push_back(std::move(statement));
}

std::string Block::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << Util::indent(indent) << name() << " [" << std::endl;

    for (auto &x : statements) {
        oss << x->dump(indent + 1) << std::endl;
    }

    oss << Util::indent(indent) << "]";
    return oss.str();
}