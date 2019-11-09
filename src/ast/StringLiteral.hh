#pragma once

#include "Literal.hh"
#include "Token.hh"

class StringLiteral : public Literal {
    MAKE_NONMOVABLE(StringLiteral)
    MAKE_NONCOPYABLE(StringLiteral)

    AST_NODE_NAME(StringLiteral)
    AST_DUMPABLE()

   public:
    StringLiteral(std::string_view value)
        : Literal(Primitive::STRING), m_value { value } {}

    const auto& value() const { return m_value; }

   private:
    // @TODO: This should be string_view but creates weird bugs...
    std::string m_value;
};