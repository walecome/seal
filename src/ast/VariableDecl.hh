#pragma once

#include "Decl.hh"
#include "Expression.hh"
#include "Token.hh"
#include "Type.hh"

class VariableDecl : public Decl {
    MAKE_NONMOVABLE(VariableDecl)
    MAKE_NONCOPYABLE(VariableDecl)

    AST_NODE_NAME(VariableDecl)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    VariableDecl(const Token identifier, const Type type,
                 ptr_t<Expression> &value, bool is_mutable)
        : m_identifier { identifier },
          m_type { type },
          m_value { std::move(value) },
          m_is_mutable { is_mutable } {}

    VariableDecl(const Token identifier, const Type type)
        : m_identifier { identifier },
          m_type { type },
          m_value { nullptr },
          m_is_mutable { false } {}

    auto is_mutable() const { return m_is_mutable; }
    const auto &type() const { return m_type; }
    const auto &identifier() const { return m_identifier; }
    const auto &value() const { return m_value; }

   private:
    const Token m_identifier;
    const Type m_type;
    ptr_t<Expression> m_value;
    bool m_is_mutable;
};