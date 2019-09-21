#pragma once

#include "Node.hh"
#include "Parameter.hh"

#include <vector>

struct ParameterList : public Node {
    virtual ~ParameterList() override;

    void addParamter(Parameter *parameter);

    std::vector<Parameter *> parameters {};
};