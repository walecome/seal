#pragma once

#include "Node.hh"
#include "VariableDecl.hh"

#include <vector>

struct ParameterList : public Node {
    void add_parameter(ptr_t<VariableDecl> &parameter);

    virtual std::string name() const override { return "ParameterList"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) override;

    std::vector<ptr_t<VariableDecl>> parameters {};
};