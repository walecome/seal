#pragma once

#include <vector>

#include "Block.hh"
#include "Node.hh"
#include "ParameterList.hh"
#include "Token.hh"

class FunctionDecl : public Node {
    MAKE_NONMOVABLE(FunctionDecl)
    MAKE_NONCOPYABLE(FunctionDecl)

    AST_NODE_NAME(FunctionDecl)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    FunctionDecl(const Token identifier, ptr_t<ParameterList> &parameters,
                 ptr_t<Block> &body, const Type type)
        : m_identifier { identifier },
          m_parameter_list { std::move(parameters) },
          m_body { std::move(body) },
          m_type { type } {}

    const auto &identifier() const { return m_identifier; }
    const auto &parameter_list() const { return m_parameter_list; }
    const auto &body() const { return m_body; }
    const auto &type() const { return m_type; }

   private:
    const Token m_identifier;
    ptr_t<ParameterList> m_parameter_list;
    ptr_t<Block> m_body;
    Type m_type;
};