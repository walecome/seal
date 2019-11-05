#pragma once

#include "Expression.hh"

struct IndexExpression : public Expression {
    IndexExpression(ptr_t<Expression> &indexed_expression,
                    ptr_t<Expression> &index)
        : m_indexed_expression { std::move(indexed_expression) },
          m_index { std::move(index) } {}

    virtual std::string name() const override { return "IndexExpression"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) override;

    ptr_t<Expression> m_indexed_expression;
    ptr_t<Expression> m_index;
};