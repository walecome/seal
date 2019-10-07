#pragma once

#include "Literal.hh"

struct ArrayLiteral : public Literal {
    void add_expression(ptr_t<Expression> &expression);

    virtual std::string name() const override { return "ArrayLiteral"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) const override;

    std::vector<ptr_t<Expression>> expressions {};
};