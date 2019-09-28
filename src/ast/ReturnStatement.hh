#pragma once

#include "Expression.hh"
#include "Statement.hh"

struct ReturnStatement : public Statement {
    ReturnStatement(ptr_t<Expression> &returnValue)
        : returnValue { std::move(returnValue) } {}

    virtual std::string name() const override { return "ReturnStatement"; }

    virtual std::string dump() const override {
        std::ostringstream oss {};
        oss << name() << " (";
        oss << returnValue->dump() << ")";
        return oss.str();
    }

    ptr_t<Expression> returnValue;
};