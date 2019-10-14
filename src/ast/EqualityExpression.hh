#pragma once

#include "BinaryExpression.hh"

struct EqualityExpression : public BinaryExpression {
    EqualityExpression(ptr_t<Expression> &left, ptr_t<Operator> &op,
                       ptr_t<Expression> &right)
        : BinaryExpression(left, op, right) {
        type = Type(Primitive::BOOL);
    }
    virtual std::string name() const override { return "EqualityExpression"; }
};