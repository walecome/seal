#pragma once

#include "Literal.hh"
#include "Token.hh"

class FloatLiteral : public Literal {
    MAKE_NONMOVABLE(FloatLiteral)
    MAKE_NONCOPYABLE(FloatLiteral)

    AST_NODE_NAME(FloatLiteral)
    AST_DUMPABLE()

    FloatLiteral(float value) : Literal(Primitive::FLOAT), m_value { value } {}

    auto value() const { return m_value; }

   private:
    float m_value;
};