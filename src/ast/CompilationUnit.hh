#pragma once

#include "FunctionDecl.hh"
#include "Node.hh"

#include <iostream>
#include <vector>

class CompilationUnit final : public Node {
   public:
    void add_function(ptr_t<FunctionDecl>& function);

    std::string dump() const;
    virtual std::string dump(unsigned indent) const override;

    void function_pass(Scope* scope);
    virtual void analyze(Scope* scope) override;

    const auto& functions() const { return m_functions; }

    // @TODO: Check why this cannot be in the source file...
    template <typename Callback>
    void for_each_function(Callback callback) const {
        for (auto& argument : m_functions) {
            callback(argument.get());
        }
    }

    void accept(const AstVisitor& visitor) override;

   private:
    std::vector<ptr_t<FunctionDecl>> m_functions {};
};
