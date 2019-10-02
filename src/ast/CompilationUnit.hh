#pragma once

#include "Function.hh"
#include "Node.hh"
#include "Scope.hh"

#include <iostream>
#include <vector>

struct CompilationUnit : public Node {
    void addFunction(ptr_t<Function>& function);
    std::vector<ptr_t<Function>> functions {};

    std::string dump() const;
    virtual std::string dump(unsigned indent) const override;

    void functionPass(Scope& scope) const;
    virtual void analyze(Scope& scope) const override;
};