#pragma once

#include <vector>

#include "Node.hh"
#include "ParameterList.hh"

struct FunctionDecl : public Node {
    FunctionDecl(const std::string &identifier,
                 ptr_t<ParameterList> &parameters)
        : identifier { identifier }, parameterList { std::move(parameters) } {}

    virtual std::string name() const override { return "FunctionDecl"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) const override;

    const std::string identifier;
    ptr_t<ParameterList> parameterList;
};