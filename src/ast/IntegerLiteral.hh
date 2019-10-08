#pragma once

#include "Literal.hh"
#include "Token.hh"

struct IntegerLiteral : public Literal {
    IntegerLiteral(const Token value)
        : Literal(Primitive::INT), value { value } {}

    virtual std::string name() const override { return "IntegerLiteral"; }
    virtual std::string dump(unsigned indent) const override;

    const Token value;
};