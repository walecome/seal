#pragma once

#include "Expression.hh"
#include "Statement.hh"

struct ReturnStatement : public Statement {
    ReturnStatement(ptr_t<Expression> &returnValue)
        : returnValue { std::move(returnValue) } {}

    virtual std::string name() const override { return "ReturnStatement"; }

    virtual std::string dump(unsigned indent) const override;
    ptr_t<Expression> returnValue;
};