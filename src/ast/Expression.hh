#pragma once

#include "Operator.hh"
#include "Statement.hh"

#include <queue>
#include <stack>

struct Expression : public Statement {
    virtual std::string name() const override { return "Expression"; }

    virtual void analyze(Scope *) const override {
        throw std::runtime_error("Analysis of pure expression not allowed");
    };
};