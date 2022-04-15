#pragma once

#include "Block.hh"
#include "Expression.hh"
#include "Statement.hh"

class IfStatement final : public Statement {
    MAKE_NONMOVABLE(IfStatement)
    MAKE_NONCOPYABLE(IfStatement)

    AST_NODE_NAME(IfStatement)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    IfStatement(ptr_t<Expression>& condition, ptr_t<Block>& if_body,
                ptr_t<Block>& else_body)
        : m_condition { std::move(condition) },
          m_if_body { std::move(if_body) },
          m_else_body { std::move(else_body) } {}

    auto condition() const { return m_condition.get(); }
    auto if_body() const { return m_if_body.get(); }
    auto else_body() const { return m_else_body.get(); }

   private:
    ptr_t<Expression> m_condition;
    ptr_t<Block> m_if_body;
    ptr_t<Block> m_else_body;
};
