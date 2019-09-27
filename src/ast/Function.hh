#pragma once

#include "Block.hh"
#include "FunctionDecl.hh"
#include "Node.hh"

struct Function : public Node {
    Function(ptr_t<FunctionDecl> &functionDecl, ptr_t<Block> &block)
        : functionDecl { std::move(functionDecl) },
          block { std::move(block) } {}

    virtual std::string name() const override { return "Function"; }

    ptr_t<FunctionDecl> functionDecl { nullptr };
    ptr_t<Block> block { nullptr };
};