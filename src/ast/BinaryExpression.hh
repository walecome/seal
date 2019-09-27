#pragma once

#include "Expression.hh"

struct BinaryExpression : public Expression {
    BinaryExpression(ptr_t<Expression> &right, ptr_t<Expression> &left,
                     ptr_t<Operator> &op)
        : right { std::move(right) },
          left { std::move(left) },
          op { std::move(op) } {}

    virtual std::string name() const override { return "BinaryExpression"; }

    ptr_t<Expression> right;
    ptr_t<Expression> left;
    ptr_t<Operator> op;
};