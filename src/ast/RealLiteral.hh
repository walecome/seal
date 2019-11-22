#pragma once

#include "Literal.hh"
#include "Token.hh"

class RealLiteral : public Literal {
    MAKE_NONMOVABLE(RealLiteral)
    MAKE_NONCOPYABLE(RealLiteral)

    AST_NODE_NAME(RealLiteral)
    AST_DUMPABLE()

    RealLiteral(float value) : Literal(Primitive::FLOAT), m_value { value } {}

    auto value() const { return m_value; }

   private:
    double m_value;
};