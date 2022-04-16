#pragma once

#include "BinaryExpression.hh"

class CompareExpression final : public BinaryExpression {
    MAKE_NONMOVABLE(CompareExpression)
    MAKE_NONCOPYABLE(CompareExpression)

    AST_NODE_NAME(CompareExpression)

   public:
    CompareExpression(ptr_t<Expression> &left, ptr_t<Operator> &op,
                      ptr_t<Expression> &right)
        : BinaryExpression(left, op, right) {
        m_type = Type(Primitive::BOOL);
    }

    void accept(const AstVisitor &visitor) override;
};
