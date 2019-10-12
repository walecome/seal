#pragma once

#include "BinaryExpression.hh"

struct CompareExpression : public BinaryExpression {
    using BinaryExpression::BinaryExpression;
    virtual std::string name() const override { return "CompareExpression"; }

    // virtual void analyze(Scope *scope) override;
};