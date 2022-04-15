#pragma once

#include "Literal.hh"
#include "Token.hh"

class BooleanLiteral final : public Literal {
    MAKE_NONMOVABLE(BooleanLiteral)
    MAKE_NONCOPYABLE(BooleanLiteral)

    AST_NODE_NAME(BooleanLiteral)
    AST_DUMPABLE()

   public:
    BooleanLiteral(bool value) : Literal(Primitive::BOOL), m_value { value } {}

    auto value() const { return m_value; }

   private:
    bool m_value;
};
