#pragma once

#include "Node.hh"
#include "Parameter.hh"

#include <vector>

struct ParameterList : public Node {
    void addParamter(ptr_t<Parameter> &parameter);

    std::vector<ptr_t<Parameter>> parameters {};
};