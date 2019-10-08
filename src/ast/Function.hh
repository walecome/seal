#pragma once

#include "Block.hh"
#include "FunctionDecl.hh"
#include "Node.hh"

struct Function : public Node {
    Function(ptr_t<FunctionDecl> &function_decl, ptr_t<Block> &block)
        : function_decl { std::move(function_decl) },
          block { std::move(block) } {}

    virtual std::string name() const override { return "Function"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) override;

    ptr_t<FunctionDecl> function_decl;
    ptr_t<Block> block;
};