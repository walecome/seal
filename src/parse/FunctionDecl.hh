#pragma once

#include <vector>

#include "Node.hh"
#include "ParameterList.hh"

struct FunctionDecl : public Node {
    FunctionDecl(ptr_t<ParameterList> &parameters)
        : parameterList { std::move(parameters) } {}

    virtual std::string name() const override { return "FunctionDecl"; }

    ptr_t<ParameterList> parameterList { nullptr };
};