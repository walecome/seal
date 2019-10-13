#pragma once

#include <vector>

#include "Block.hh"
#include "Node.hh"
#include "ParameterList.hh"
#include "Token.hh"

struct FunctionDecl : public Node {
    FunctionDecl(const Token identifier, ptr_t<ParameterList> &parameters,
                 ptr_t<Block> &body, const Type type)
        : identifier { identifier },
          parameter_list { std::move(parameters) },
          body { std::move(body) },
          type { type } {}

    virtual std::string name() const override { return "FunctionDecl"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) override;

    const Token identifier;
    ptr_t<ParameterList> parameter_list;
    ptr_t<Block> body;
    Type type;
};