#pragma once

#include "Node.hh"
#include "Parameter.hh"

#include <vector>

struct ParameterList : public Node {
    void addParamter(ptr_t<Parameter> &parameter);

    virtual std::string name() const override { return "ParameterList"; }

    virtual std::string dump(unsigned indent) const override;

    std::vector<ptr_t<Parameter>> parameters {};
};