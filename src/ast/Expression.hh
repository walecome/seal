#pragma once

#include "Node.hh"

struct Expression : public Node {
    virtual std::string name() const override { return "Expression"; }
};