#pragma once

#include "Block.hh"
#include "Expression.hh"
#include "Statement.hh"
#include "VariableDecl.hh"

class For : public Statement {
    AST_NODE_NAME(For)
    MAKE_NONCOPYABLE(For)
    MAKE_NONMOVABLE(For)

   public:
    For(ptr_t<VariableDecl> &initial_expression,
        ptr_t<Expression> &stop_condition,
        ptr_t<Expression> &per_iteration_expression, ptr_t<Block> &body)
        : m_initial_expression { std::move(initial_expression) },
          m_stop_condition { std::move(stop_condition) },
          m_per_iteration_expression { std::move(per_iteration_expression) },
          m_body { std::move(body) } {}

    virtual void analyze(Scope *scope) override;
    virtual std::string dump(unsigned indent) const override;

    auto initial_expression() const { return m_initial_expression.get(); }
    auto stop_condition() const { return m_stop_condition.get(); }
    auto per_iteration_expression() const {
        return m_per_iteration_expression.get();
    }
    auto body() const { return m_body.get(); }

   private:
    // This should be something else than a variable declaration
    ptr_t<VariableDecl> m_initial_expression;
    ptr_t<Expression> m_stop_condition;
    ptr_t<Expression> m_per_iteration_expression;
    ptr_t<Block> m_body;
};