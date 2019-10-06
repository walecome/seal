#pragma once

#include "Literal.hh"

struct StringLiteral : public Literal {
    StringLiteral(const std::string_view value) : value { value } {}

    virtual std::string name() const override { return "StringLiteral"; }
    virtual std::string dump(unsigned indent) const override;

    const std::string_view value;
};