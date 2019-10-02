#pragma once

#include "Node.hh"
struct Statement : public Node {
    virtual std::string name() const override { return "Statement"; }
    virtual std::string dump(unsigned) const override {
        throw std::runtime_error("Pure statement node not allowed");
    };
    virtual void analyze(Scope *) const override {
        throw std::runtime_error("Analysis of pure statement not allowed");
    };
};