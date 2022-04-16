#pragma once

#include "Expression.hh"

class BinaryExpression : public Expression {
    MAKE_NONMOVABLE(BinaryExpression)
    MAKE_NONCOPYABLE(BinaryExpression)

    AST_NODE_NAME(BinaryExpression)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   protected:
    BinaryExpression(ptr_t<Expression>& left, ptr_t<Operator>& op,
                     ptr_t<Expression>& right)
        : m_left { std::move(left) },
          m_operator { std::move(op) },
          m_right { std::move(right) } {}

   public:
    auto left() const { return m_left.get(); }
    auto op() const { return m_operator.get(); }
    auto right() const { return m_right.get(); }

   protected:
    ptr_t<Expression> m_left;
    ptr_t<Operator> m_operator;
    ptr_t<Expression> m_right;
};
