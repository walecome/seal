#pragma once

#include "Expression.hh"

class Literal : public Expression {
    using Expression::Expression;

    MAKE_NONMOVABLE(Literal)
    MAKE_NONCOPYABLE(Literal)

    AST_NODE_NAME(Literal)

    virtual bool is_literal() const override { return true; }

    virtual void analyze(Scope *) override {};
    virtual std::string dump(unsigned index) const override;
};
