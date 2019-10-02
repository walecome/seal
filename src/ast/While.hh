#pragma once

#include "Block.hh"
#include "Expression.hh"
#include "Statement.hh"

struct While : public Statement {
    While(ptr_t<Expression> &condition, ptr_t<Block> &body)
        : condition { std::move(condition) }, body { std::move(body) } {}

    virtual std::string name() const override { return "While"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) const override;

    ptr_t<Expression> condition;
    ptr_t<Block> body;
};