#pragma once

#include <vector>

#include "Node.hh"
#include "ParameterList.hh"
#include "Token.hh"

struct FunctionDecl : public Node {
    FunctionDecl(const Token identifier, ptr_t<ParameterList> &parameters,
                 const Type type)
        : identifier { identifier },
          parameter_list { std::move(parameters) },
          type { type } {}

    virtual std::string name() const override { return "FunctionDecl"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) override;

    const Token identifier;
    ptr_t<ParameterList> parameter_list;

    Type type;
};