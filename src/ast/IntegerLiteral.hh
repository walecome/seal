#pragma once

#include "Literal.hh"

struct IntegerLiteral : public Literal {
    IntegerLiteral(int value) : Literal(Primitive::INT), value { value } {}

    virtual std::string name() const override { return "IntegerLiteral"; }
    virtual std::string dump(unsigned indent) const override;

    int value;
};