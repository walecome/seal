#pragma once

#include "Block.hh"
#include "Expression.hh"
#include "Statement.hh"

class While : public Statement {
    MAKE_NONMOVABLE(While)
    MAKE_NONCOPYABLE(While)

    AST_NODE_NAME(While)
    AST_ANALYZABLE()
    AST_DUMPABLE()

   public:
    While(ptr_t<Expression>& condition, ptr_t<Block>& body)
        : m_condition { std::move(condition) }, m_body { std::move(body) } {}

    const auto& condition() const { return m_condition; }
    const auto& body() const { return m_body; }

   private:
    ptr_t<Expression> m_condition;
    ptr_t<Block> m_body;
};