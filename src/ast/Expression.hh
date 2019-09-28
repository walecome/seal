#pragma once

#include "Operator.hh"
#include "Statement.hh"

#include <queue>
#include <stack>

struct Expression : public Statement {
    virtual std::string name() const override { return "Expression"; }
};