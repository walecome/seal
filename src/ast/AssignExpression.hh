#pragma once

#include "BinaryExpression.hh"

struct AssignExpression : public BinaryExpression {
    using BinaryExpression::BinaryExpression;
    virtual std::string name() const override { return "AssignExpression"; }

    virtual void analyze(Scope *scope) override;
};