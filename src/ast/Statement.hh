#pragma once

#include "Node.hh"
struct Statement : public Node {
    virtual std::string name() const override { return "Statement"; }
    virtual std::string dump() const override { return name(); }
};