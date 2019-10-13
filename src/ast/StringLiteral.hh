#pragma once

#include "Literal.hh"
#include "Token.hh"

struct StringLiteral : public Literal {
    StringLiteral(std::string_view value)
        : Literal(Primitive::STRING), value { value } {}
    virtual std::string name() const override { return "StringLiteral"; }
    virtual std::string dump(unsigned indent) const override;

    // @TODO: This should be string_view but creates weird bugs...
    std::string value;
};