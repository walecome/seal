#pragma once

#include "Node.hh"
#include "VariableDecl.hh"

#include <vector>

struct ParameterList : public Node {
    void addParameter(ptr_t<VariableDecl> &parameter);

    virtual std::string name() const override { return "ParameterList"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) const override;

    std::vector<ptr_t<VariableDecl>> parameters {};
};