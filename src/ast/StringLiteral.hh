#pragma once

#include "Literal.hh"
#include "Token.hh"

struct StringLiteral : public Literal {
    StringLiteral(const Token value)
        : Literal(Primitive::STRING), value { value } {}

    virtual std::string name() const override { return "StringLiteral"; }
    virtual std::string dump(unsigned indent) const override;

    const Token value;
};