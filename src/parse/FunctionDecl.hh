#pragma once

#include <vector>

#include "Node.hh"
#include "ParameterList.hh"

struct FunctionDecl : public Node {
    FunctionDecl(ParameterList *parameters) : parameterList { parameters } {}

    virtual ~FunctionDecl() override;

    virtual std::string name() const override { return "FunctionDecl"; }

    ParameterList *parameterList { nullptr };
};