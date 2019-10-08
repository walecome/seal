#pragma once

#include "Expression.hh"

struct VariableExpression : public Expression {
    VariableExpression(const std::string_view identifier)
        : identifier { identifier } {}

    virtual std::string name() const override { return "VariableExpression"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) override;

    const std::string_view identifier;
};