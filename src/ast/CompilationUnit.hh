#pragma once

#include "FunctionDecl.hh"
#include "Node.hh"

#include <iostream>
#include <vector>

class CompilationUnit : public Node {
   public:
    void add_function(ptr_t<FunctionDecl>& function);

    std::string dump() const;
    virtual std::string dump(unsigned indent) const override;

    void function_pass(Scope* scope);
    virtual void analyze(Scope* scope) override;

    const auto& functions() const { return m_functions; }

   private:
    std::vector<ptr_t<FunctionDecl>> m_functions {};
};