#pragma once

#include "Literal.hh"
#include "Token.hh"

class IntegerLiteral final : public Literal {
    MAKE_NONMOVABLE(IntegerLiteral)
    MAKE_NONCOPYABLE(IntegerLiteral)

    AST_NODE_NAME(IntegerLiteral)
    AST_DUMPABLE()

    const auto& value() const { return m_value; }

   public:
    IntegerLiteral(int value) : Literal(Primitive::INT), m_value { value } {}

    void accept(const AstVisitor& visitor) override;

   private:
    int m_value;
};
