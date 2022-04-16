#pragma once

#include "ast/BinaryExpression.hh"

class CalculateExpression final : public BinaryExpression {
    MAKE_NONMOVABLE(CalculateExpression)
    MAKE_NONCOPYABLE(CalculateExpression)

    AST_NODE_NAME(CalculateExpression)
    AST_ANALYZABLE()

   public:
    CalculateExpression(ptr_t<Expression>& left, ptr_t<Operator>& op,
                        ptr_t<Expression>& right)
        : BinaryExpression(left, op, right) {}

    void accept(const AstVisitor& visitor) override;

   protected:
    ptr_t<Expression> m_left;
    ptr_t<Operator> m_operator;
    ptr_t<Expression> m_right;
};
