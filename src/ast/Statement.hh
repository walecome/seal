#pragma once

#include "Node.hh"
class Statement : public Node {
   public:
    virtual std::string name() const override { return "Statement"; }
    virtual std::string dump(unsigned) const override {
        throw std::runtime_error("Pure statement node not allowed");
    };
    virtual void analyze(Scope *) override {
        throw std::runtime_error("Analysis of pure statement not allowed");
    };
};