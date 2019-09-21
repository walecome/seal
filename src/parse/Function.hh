#pragma once

#include "Node.hh"

struct Function : public Node {
    virtual std::string name() const override { return "Function"; }
};