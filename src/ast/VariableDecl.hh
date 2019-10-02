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

    virtual std::string name() const override { return "VariableDecl"; }
    virtual std::string dump(unsigned indent) const override;

    const std::string identifier;
    const Type type;
    ptr_t<Expression> value;
};