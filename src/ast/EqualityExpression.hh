#pragma once

#include "BinaryExpression.hh"

class EqualityExpression final : public BinaryExpression {
    MAKE_NONMOVABLE(EqualityExpression)
    MAKE_NONCOPYABLE(EqualityExpression)

    AST_NODE_NAME(EqualityExpression)

   public:
    EqualityExpression(ptr_t<Expression> &left, ptr_t<Operator> &op,
                       ptr_t<Expression> &right)
        : BinaryExpression(left, op, right) {
        m_type = Type(Primitive::BOOL);
    }
};
