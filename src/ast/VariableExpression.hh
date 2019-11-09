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
    VariableExpression(const Token identifier) : m_identifier { identifier } {}

    const auto& identifier() const { return m_identifier; }

   private:
    const Token m_identifier;
};