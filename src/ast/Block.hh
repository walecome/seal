#pragma once

#include "Node.hh"
#include "Statement.hh"

#include <sstream>
#include <vector>

class Block : public Statement {
    AST_NODE_NAME(Block)
    MAKE_NONMOVABLE(Block)
    MAKE_NONCOPYABLE(Block)

   public:
    Block() = default;

    virtual std::string dump(unsigned indent) const override;
    void add_node(ptr_t<Node> &node);
    virtual void analyze(Scope *scope) override;

    const auto &nodes() const { return m_nodes; }

   private:
    std::vector<ptr_t<Node>> m_nodes {};
};