#pragma once

#include <vector>

#include "Expression.hh"
#include "Node.hh"

struct ArgumentList : public Node {
    virtual std::string name() const override { return "ArgumentList"; }

    virtual std::string dump(unsigned indent) const override;
    void add_argument(ptr_t<Expression> &expression);

    template <typename Function>
    void for_each_argument(Function f) const;

    virtual void analyze(Scope *scope) override;

    std::vector<ptr_t<Expression>> arguments {};
};
