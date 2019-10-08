#pragma once

#include "Expression.hh"
#include "Token.hh"

struct VariableExpression : public Expression {
    VariableExpression(const Token identifier) : identifier { identifier } {}

    virtual std::string name() const override { return "VariableExpression"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) override;

    const Token identifier;
};