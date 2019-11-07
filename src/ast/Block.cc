#include "Block.hh"

void Block::add_node(ptr_t<Node> &statement) {
    m_nodes.push_back(std::move(statement));
}

void Block::analyze(Scope *scope) {
    ptr_t<Scope> inner_scope = std::make_unique<Scope>(scope);

    for (auto &node : m_nodes) {
        node->analyze(inner_scope.get());
    }
}

std::string Block::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " [" << std::endl;

    for (auto &x : m_nodes) {
        oss << x->dump(indent + 1) << std::endl;
    }

    oss << util::indent(indent) << "]";
    return oss.str();
}