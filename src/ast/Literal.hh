#pragma once

#include "Expression.hh"

struct Literal : public Expression {
    Literal() = default;
    virtual std::string name() const override { return "Literal"; }
    virtual void analyze(Scope *) const override {};
};