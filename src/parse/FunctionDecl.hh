#pragma once

#include "Node.hh"

struct FunctionDecl : public Node {
    virtual std::string name() const override { return "FunctionDecl"; }
};