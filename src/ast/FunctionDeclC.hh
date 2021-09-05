
#pragma once

#include <string_view>
#include <vector>

#include "Block.hh"
#include "FunctionDecl.hh"
#include "ParameterList.hh"
#include "Scope.hh"
#include "Token.hh"

class FunctionDeclC : public FunctionDecl {
    MAKE_NONMOVABLE(FunctionDeclC)
    MAKE_NONCOPYABLE(FunctionDeclC)

    AST_NODE_NAME(FunctionDeclC)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    FunctionDeclC(const std::string_view identifier,
                     ptr_t<ParameterList>& parameters, const Type type,
                     const std::string_view lib_name)
        : FunctionDecl(identifier, parameters, type), m_lib_name(lib_name) {}

    std::string_view lib_name() const;

   private:
   const std::string_view m_lib_name;
};