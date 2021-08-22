#pragma once

#include <vector>

#include "Block.hh"
#include "FunctionDecl.hh"
#include "ParameterList.hh"
#include "Scope.hh"
#include "Token.hh"

class FunctionDeclUser : public FunctionDecl {
    MAKE_NONMOVABLE(FunctionDeclUser)
    MAKE_NONCOPYABLE(FunctionDeclUser)

    AST_NODE_NAME(FunctionDeclUser)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    FunctionDeclUser(const std::string_view identifier,
                     ptr_t<ParameterList>& parameters, const Type type,
                     ptr_t<Block>& block)
        : FunctionDecl(identifier, parameters, type),
          m_body(std::move(block)) {}

    auto* body() const { return m_body.get(); }

   private:
    ptr_t<Block> m_body;
};