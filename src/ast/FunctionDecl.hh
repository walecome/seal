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

   public:
    FunctionDecl(const std::string_view identifier,
                 ptr_t<ParameterList> &parameters, const Type type)
        : m_identifier { identifier },
          m_parameter_list { std::move(parameters) },
          m_type { type },
          m_id { new_function_id() } {}

    const auto &identifier() const { return m_identifier; }
    auto parameter_list() const { return m_parameter_list.get(); }
    const auto &type() const { return m_type; }

    int function_id() const { return m_id; }

   protected:
    const std::string_view m_identifier;
    ptr_t<ParameterList> m_parameter_list;
    Type m_type;
    int m_id;
};