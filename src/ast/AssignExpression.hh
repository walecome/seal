#pragma once

#include "BinaryExpression.hh"

class AssignExpression final : public BinaryExpression {
    MAKE_NONMOVABLE(AssignExpression)
    MAKE_NONCOPYABLE(AssignExpression)

    AST_NODE_NAME(AssignExpression)
    AST_ANALYZABLE()

   public:
    using BinaryExpression::BinaryExpression;
};
