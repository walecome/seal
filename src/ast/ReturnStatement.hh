#pragma once

#include "Expression.hh"
#include "Statement.hh"

struct ReturnStatement : public Statement {
    ReturnStatement(ptr_t<Expression> &return_value)
        : return_value { std::move(return_value) } {}

    virtual std::string name() const override { return "ReturnStatement"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) override;

    ptr_t<Expression> return_value;
};