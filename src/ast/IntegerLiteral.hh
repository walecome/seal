#pragma once

#include "Literal.hh"

struct IntegerLiteral : public Literal {
    IntegerLiteral(int value) : value { value } {}

    virtual std::string name() const override { return "IntegerLiteral"; }

    virtual std::string dump() const override {
        std::ostringstream oss {};
        oss << "{" << name() << ", " << value << "}";
        return oss.str();
    }

    int value;
};