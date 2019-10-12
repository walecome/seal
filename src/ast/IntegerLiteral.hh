#pragma once

#include "Literal.hh"
#include "Token.hh"

struct IntegerLiteral : public Literal {
    IntegerLiteral(const Token value) : Literal(Primitive::INT, value) {}
    virtual std::string name() const override { return "IntegerLiteral"; }
};