#pragma once

#include "Node.hh"
#include "Operator.hh"

#include <queue>
#include <stack>

struct Expression : public Node {
    virtual std::string name() const override { return "Expression"; }
};