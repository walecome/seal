#pragma once

#include "Literal.hh"
#include "Token.hh"
#include "utility/StringTable.hh"

class StringLiteral final : public Literal {
    MAKE_NONMOVABLE(StringLiteral)
    MAKE_NONCOPYABLE(StringLiteral)

    AST_NODE_NAME(StringLiteral)
    AST_DUMPABLE()

   public:
    StringLiteral(StringTable::Key value)
        : Literal(Primitive::STRING), m_value { value } {}

    auto value() const { return m_value; }

   private:
    StringTable::Key m_value;
};
