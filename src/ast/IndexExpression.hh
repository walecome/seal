#pragma once

#include "Expression.hh"

class IndexExpression : public Expression {
    MAKE_NONMOVABLE(IndexExpression)
    MAKE_NONCOPYABLE(IndexExpression)

    AST_NODE_NAME(IndexExpression)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    IndexExpression(ptr_t<Expression> &indexed_expression,
                    ptr_t<Expression> &index)
        : m_indexed_expression { std::move(indexed_expression) },
          m_index { std::move(index) } {}

    const auto &indexed_expression() const { return m_indexed_expression; }

   private:
    ptr_t<Expression> m_indexed_expression;
    ptr_t<Expression> m_index;
};