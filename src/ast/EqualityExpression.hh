#pragma once

#include "BinaryExpression.hh"

struct EqualityExpression : public BinaryExpression {
    using BinaryExpression::BinaryExpression;
    virtual std::string name() const override { return "EqualityExpression"; }

    // virtual void analyze(Scope *scope) override;
};