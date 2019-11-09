#pragma once

#include "Node.hh"
#include "Statement.hh"

#include <sstream>
#include <vector>

class Block : public Statement {
    MAKE_DEFAULT_CONSTRUCTABLE(Block)
    MAKE_NONMOVABLE(Block)
    MAKE_NONCOPYABLE(Block)

    AST_NODE_NAME(Block)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    void add_node(ptr_t<Node> &node);
    const auto &nodes() const { return m_nodes; }

   private:
    std::vector<ptr_t<Node>> m_nodes {};
};