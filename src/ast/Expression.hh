#pragma once

#include "Operator.hh"
#include "Statement.hh"
#include "Type.hh"

#include <queue>
#include <stack>

class Expression : public Statement {
    MAKE_DEFAULT_CONSTRUCTABLE(Expression)
    MAKE_NONMOVABLE(Expression)
    MAKE_NONCOPYABLE(Expression)

    AST_NODE_NAME(Expression)

   public:
    Expression(Type type) : m_type { type } {}

    virtual void analyze(Scope *) override {
        throw std::runtime_error("Analysis of pure expression not allowed");
    };

    void set_primitive(const Primitive primitive);
    void set_type(Type type);

    const auto &type() const { return m_type; }

   protected:
    Type m_type {};
};
