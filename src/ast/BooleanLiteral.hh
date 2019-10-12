#pragma once

#include "Literal.hh"
#include "Token.hh"

struct BooleanLiteral : public Literal {
    BooleanLiteral(const Token value) : Literal(Primitive::BOOL, value) {}

    virtual std::string name() const override { return "BooleanLiteral"; }
};