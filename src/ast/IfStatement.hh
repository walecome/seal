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

    virtual std::string dump() const override {
        std::ostringstream oss {};
        oss << name() << " (";
        oss << condition->dump() << ", " << ifBody->dump() << ", ";
        if (elseBody) {
            oss << elseBody->dump();
        } else {
            oss << "NULL ELSEBODY";
        }
        oss << ")";
        return oss.str();
    }

    ptr_t<Expression> condition;
    ptr_t<Block> ifBody;
    ptr_t<Block> elseBody;
};