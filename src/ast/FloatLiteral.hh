#pragma once

#include "Literal.hh"
#include "Token.hh"

struct FloatLiteral : public Literal {
    FloatLiteral(float value) : Literal(Primitive::FLOAT), value { value } {}

    virtual std::string name() const override { return "FloatLiteral"; }
    virtual std::string dump(unsigned indent) const override;

    float value;
};