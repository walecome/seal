#pragma once

#include "FunctionDecl.hh"
#include "Node.hh"

#include <iostream>
#include <vector>

struct CompilationUnit : public Node {
    void add_function(ptr_t<FunctionDecl>& function);
    std::vector<ptr_t<FunctionDecl>> functions {};

    std::string dump() const;
    virtual std::string dump(unsigned indent) const override;

    void function_pass(Scope* scope);
    virtual void analyze(Scope* scope) override;
};