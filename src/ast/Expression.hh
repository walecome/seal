#pragma once

#include "Operator.hh"
#include "Statement.hh"
#include "Type.hh"

#include <queue>
#include <stack>

struct Expression : public Statement {
    Expression() = default;
    Expression(Type type) : type { type } {}

    virtual std::string name() const override { return "Expression"; }

    virtual void analyze(Scope *) override {
        throw std::runtime_error("Analysis of pure expression not allowed");
    };

    Type type {};
};