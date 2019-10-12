#pragma once

#include "Expression.hh"

struct Literal : public Expression {
    Literal() = default;
    virtual std::string name() const override { return "Literal"; }
    virtual std::string dump(unsigned indent) const override;
    virtual void analyze(Scope *) override {};

    const Token value;

   protected:
    Literal(Primitive primitive, const Token value) : value { value } {
        type = Type(primitive);
    }
};