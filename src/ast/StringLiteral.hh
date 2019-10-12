#pragma once

#include "Literal.hh"
#include "Token.hh"

struct StringLiteral : public Literal {
    StringLiteral(const Token value) : Literal(Primitive::STRING, value) {}
    virtual std::string name() const override { return "StringLiteral"; }
};