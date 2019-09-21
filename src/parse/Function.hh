#pragma once

#include "Block.hh"
#include "FunctionDecl.hh"
#include "Node.hh"

struct Function : public Node {
    Function(FunctionDecl* functionDecl, Block* block)
        : functionDecl { functionDecl }, block { block } {}

    virtual ~Function() override;

    virtual std::string name() const override { return "Function"; }

    FunctionDecl* functionDecl { nullptr };
    Block* block { nullptr };
};