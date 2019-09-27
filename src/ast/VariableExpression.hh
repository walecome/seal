#pragma once

#include "Expression.hh"

struct VariableExpression : public Expression {
    VariableExpression(const std::string identifier)
        : identifier { identifier } {}

    virtual std::string name() const override { return "VariableExpression"; }

    const std::string identifier;
};