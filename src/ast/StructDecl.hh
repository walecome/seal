#pragma once

#include <vector>

#include "Token.hh"

#include "ast/Block.hh"
#include "ast/Node.hh"
#include "ast/ParameterList.hh"
#include "ast/MemberFunctionDecl.hh"

class StructDecl : public Node {
    MAKE_NONMOVABLE(StructDecl)
    MAKE_NONCOPYABLE(StructDecl)

    AST_NODE_NAME(StructDecl)
    AST_DUMPABLE()

   public:
    StructDecl(
        const std::string_view identifier,
        std::vector<ptr_t<VariableDecl>>& members,
        std::vector<ptr_t<MemberFunctionDecl>>& member_functions
    ) : m_identifier(identifier), m_members(std::move(members)), m_member_functions(std::move(member_functions)) {}

    const std::string_view &identifier() const { return m_identifier; }
    const std::vector<ptr_t<VariableDecl>>& member() { return m_members; }
    const std::vector<ptr_t<MemberFunctionDecl>>& member_functions() { return m_member_functions; }

   protected:
    const std::string_view m_identifier;
    const std::vector<ptr_t<VariableDecl>> m_members;
    const std::vector<ptr_t<MemberFunctionDecl>> m_member_functions;
};
