#pragma once

#include "Expression.hh"

struct Literal : public Expression {
    using Expression::Expression;
    virtual std::string name() const override { return "Literal"; }
    virtual void analyze(Scope *) override {};
};