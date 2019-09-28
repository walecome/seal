#pragma once

#include "Expression.hh"

struct VariableExpression : public Expression {
    VariableExpression(const std::string identifier)
        : identifier { identifier } {}

    virtual std::string name() const override { return "VariableExpression"; }

    virtual std::string dump() const override {
        std::ostringstream oss {};
        oss << "[" << name() << ", " << identifier << "]";
        return oss.str();
    }

    const std::string identifier;
};