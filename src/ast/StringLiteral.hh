#pragma once

#include "Literal.hh"

struct StringLiteral : public Literal {
    StringLiteral(const std::string &value) : value { value } {}

    virtual std::string name() const override { return "StringLiteral"; }

    virtual std::string dump() const override {
        std::ostringstream oss {};
        oss << "[" << name() << ", " << value << "]";
        return oss.str();
    }

    const std::string value;
};