#pragma once

#include "Node.hh"
#include "Type.hh"

struct Parameter : public Node {
    Parameter(const std::string identifier, const Type type)
        : identifier { identifier }, type { type } {}

    virtual std::string name() const override { return "Parameter"; }

    const std::string identifier;
    const Type type;
};