#pragma once

#include "Expression.hh"
#include "Token.hh"

class VariableExpression : public Expression {
    MAKE_NONMOVABLE(VariableExpression)
    MAKE_NONCOPYABLE(VariableExpression)

    AST_NODE_NAME(VariableExpression)
    AST_ANALYZABLE()
    AST_DUMPABLE()

   public:
    VariableExpression(const std::string_view identifier)
        : m_identifier { identifier } {}

    auto identifier() const { return m_identifier; }

   private:
    const std::string_view m_identifier;
};