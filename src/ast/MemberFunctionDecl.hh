
#pragma once

#include <string_view>
#include <vector>

#include "Block.hh"
#include "FunctionDecl.hh"
#include "ParameterList.hh"
#include "Scope.hh"
#include "Token.hh"

class MemberFunctionDecl : public FunctionDecl {
    MAKE_NONMOVABLE(MemberFunctionDecl)
    MAKE_NONCOPYABLE(MemberFunctionDecl)

    AST_NODE_NAME(MemberFunctionDecl)
    AST_DUMPABLE()

   public:
    MemberFunctionDecl(
        const std::string_view identifier,
        ptr_t<ParameterList>& parameters, const Type type
    ) : FunctionDecl(identifier, parameters, type) {}
};
