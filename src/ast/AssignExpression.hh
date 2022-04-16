#pragma once

#include "ast/BinaryExpression.hh"

class AssignExpression final : public BinaryExpression {
    MAKE_NONMOVABLE(AssignExpression)
    MAKE_NONCOPYABLE(AssignExpression)

    AST_NODE_NAME(AssignExpression)
    AST_ANALYZABLE()

   public:
    AssignExpression(ptr_t<Expression>& left, ptr_t<Operator>& op,
                     ptr_t<Expression>& right)
        : BinaryExpression(left, op, right) {}

    void accept(const AstVisitor& visitor) override;
};
