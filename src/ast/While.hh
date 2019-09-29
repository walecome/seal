#pragma once

#include "Block.hh"
#include "Expression.hh"
#include "Statement.hh"

struct While : public Statement {
    While(ptr_t<Expression> &condition, ptr_t<Block> &body)
        : condition { std::move(condition) }, body { std::move(body) } {}

    virtual std::string name() const override { return "While"; }

    virtual std::string dump() const override {
        std::ostringstream oss {};
        oss << name() << " (";
        oss << condition->dump() << ", " << body->dump() << ")";
        return oss.str();
    }

    ptr_t<Expression> condition;
    ptr_t<Block> body;
};