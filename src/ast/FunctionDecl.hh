#pragma once

#include <vector>

#include "Block.hh"
#include "Node.hh"
#include "ParameterList.hh"
#include "Token.hh"

int new_function_id();

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
          m_type { type },
          m_id { new_function_id() } {}

    const auto &identifier() const { return m_identifier; }
    auto parameter_list() const { return m_parameter_list.get(); }
    auto body() const { return m_body.get(); }
    const auto &type() const { return m_type; }

    auto function_name() const { return m_identifier.value; }
    int function_id() const { return m_id; }

   private:
    const Token m_identifier;
    ptr_t<ParameterList> m_parameter_list;
    ptr_t<Block> m_body;
    Type m_type;
    int m_id;
};