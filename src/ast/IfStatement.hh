#pragma once

#include "Block.hh"
#include "Expression.hh"
#include "Statement.hh"

struct IfStatement : public Statement {
    IfStatement(ptr_t<Expression> &condition, ptr_t<Block> &if_body,
                ptr_t<Block> &else_body)
        : condition { std::move(condition) },
          if_body { std::move(if_body) },
          else_body { std::move(else_body) } {}

    virtual std::string name() const override { return "IfStatement"; }

    virtual void analyze(Scope *scope) override;

    virtual std::string dump(unsigned indent) const override;
    ptr_t<Expression> condition;
    ptr_t<Block> if_body;
    ptr_t<Block> else_body;
};