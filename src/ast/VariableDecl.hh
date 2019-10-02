#pragma once

#include "Decl.hh"
#include "Expression.hh"
#include "Type.hh"

struct VariableDecl : public Decl {
    VariableDecl(const std::string &identifier, const std::string &type,
                 ptr_t<Expression> &value)
        : identifier { identifier },
          type { type },
          value { std::move(value) } {}

    VariableDecl(const std::string &identifier, const std::string &type)
        : identifier { identifier }, type { type }, value { nullptr } {}

    virtual std::string name() const override { return "VariableDecl"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) const override;

    const std::string identifier;
    const Type type;
    ptr_t<Expression> value;
};