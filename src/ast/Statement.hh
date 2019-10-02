#pragma once

#include "Node.hh"
struct Statement : public Node {
    virtual std::string name() const override { return "Statement"; }
    virtual std::string dump(unsigned indent) const override {
        throw std::runtime_error("Pure statement node not allowed");
    };
};