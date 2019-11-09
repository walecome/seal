#pragma once

#include "Literal.hh"

class ArrayLiteral : public Literal {
    MAKE_DEFAULT_CONSTRUCTABLE(ArrayLiteral)
    MAKE_NONMOVABLE(ArrayLiteral)
    MAKE_NONCOPYABLE(ArrayLiteral)

    AST_NODE_NAME(ArrayLiteral)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    void add_expression(ptr_t<Expression>& expression);
    const auto& expressions() const { return m_expressions; }

   private:
    std::vector<ptr_t<Expression>> m_expressions {};
};