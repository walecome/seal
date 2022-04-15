#pragma once

#include "Expression.hh"

class UnaryExpression final : public Expression {
    MAKE_NONMOVABLE(UnaryExpression)
    MAKE_NONCOPYABLE(UnaryExpression)

    AST_NODE_NAME(UnaryExpression)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    UnaryExpression(ptr_t<Operator> &op, ptr_t<Expression> &expression)
        : m_operator { std::move(op) },
          m_expression { std::move(expression) } {}

    auto op() const { return m_operator.get(); }
    auto expression() const { return m_expression.get(); }

   private:
    ptr_t<Operator> m_operator;
    ptr_t<Expression> m_expression;
};
