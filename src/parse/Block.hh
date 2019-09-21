#pragma once

#include "Node.hh"

struct Block : public Node {
    virtual std::string name() const override { return "Block"; }
};