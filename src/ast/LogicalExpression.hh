#pragma once

#include "BinaryExpression.hh"

class LogicalExpression : public BinaryExpression {
    MAKE_NONMOVABLE(LogicalExpression)
    MAKE_NONCOPYABLE(LogicalExpression)

    AST_NODE_NAME(LogicalExpression)

   public:
    LogicalExpression(ptr_t<Expression> &left, ptr_t<Operator> &op,
                      ptr_t<Expression> &right)
        : BinaryExpression(left, op, right) {
        m_type = Type(Primitive::BOOL);
    }
};