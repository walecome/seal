#pragma once

#include "Expression.hh"

struct UnaryExpression : public Expression {
    UnaryExpression(ptr_t<Operator> &op, ptr_t<Expression> &expression)
        : op { std::move(op) }, expression { std::move(expression) } {}

    virtual std::string name() const override { return "UnaryExpression"; }
    virtual std::string dump(unsigned indent) const override;
    virtual void analyze(Scope *scope) override;

    ptr_t<Operator> op;
    ptr_t<Expression> expression;
};