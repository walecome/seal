#pragma once

#include "Block.hh"
#include "Expression.hh"
#include "Statement.hh"

struct IfStatement : public Statement {
    IfStatement(ptr_t<Expression> &condition, ptr_t<Block> &ifBody,
                ptr_t<Block> &elseBody)
        : condition { std::move(condition) },
          ifBody { std::move(ifBody) },
          elseBody { std::move(elseBody) } {}

    virtual std::string name() const override { return "IfStatement"; }

    virtual void analyze(Scope *scope) const override;

    virtual std::string dump(unsigned indent) const override;
    ptr_t<Expression> condition;
    ptr_t<Block> ifBody;
    ptr_t<Block> elseBody;
};