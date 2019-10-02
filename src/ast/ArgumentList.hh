#pragma once

#include <vector>

#include "Expression.hh"
#include "Node.hh"

struct ArgumentList : public Node {
    virtual std::string name() const override { return "ArgumentList"; }

    virtual std::string dump(unsigned indent) const override;
    void addArgument(ptr_t<Expression> &expression);

    virtual void analyze(Scope *scope) const override;

    std::vector<ptr_t<Expression>> arguments {};
};