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
    StringLiteral(std::string_view value)
        : Literal(Primitive::STRING), m_value { value } {}

    auto value() const { return m_value; }

   private:
    std::string_view m_value;
};
