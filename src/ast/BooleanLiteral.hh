#pragma once

#include "Literal.hh"
#include "Token.hh"

struct BooleanLiteral : public Literal {
    BooleanLiteral(bool value) : Literal(Primitive::BOOL), value { value } {}

    virtual std::string name() const override { return "BooleanLiteral"; }
    virtual std::string dump(unsigned indent) const override;

    bool value;
};