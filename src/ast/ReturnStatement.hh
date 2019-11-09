#pragma once

#include "Expression.hh"
#include "Statement.hh"

class ReturnStatement : public Statement {
    MAKE_NONMOVABLE(ReturnStatement)
    MAKE_NONCOPYABLE(ReturnStatement)

    AST_NODE_NAME(ReturnStatement)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    ReturnStatement(ptr_t<Expression>& return_value)
        : m_return_value { std::move(return_value) } {}

    const auto& return_value() const { return m_return_value; }

   private:
    ptr_t<Expression> m_return_value;
};