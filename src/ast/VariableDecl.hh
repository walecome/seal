#pragma once

#include "Decl.hh"
#include "Expression.hh"
#include "Token.hh"
#include "Type.hh"

struct VariableDecl : public Decl {
    VariableDecl(const Token identifier, const std::string_view type,
                 ptr_t<Expression> &value)
        : identifier { identifier },
          type { type },
          value { std::move(value) } {}

    VariableDecl(const Token identifier, const std::string_view type)
        : identifier { identifier }, type { type }, value { nullptr } {}

    virtual std::string name() const override { return "VariableDecl"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) override;

    const Token identifier;
    const Type type;
    ptr_t<Expression> value;
};