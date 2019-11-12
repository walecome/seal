#pragma once

#include "Node.hh"
#include "VariableDecl.hh"

#include <vector>

class ParameterList : public Node {
    MAKE_DEFAULT_CONSTRUCTABLE(ParameterList)
    MAKE_NONMOVABLE(ParameterList)
    MAKE_NONCOPYABLE(ParameterList)

    AST_NODE_NAME(ParameterList)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    void add_parameter(ptr_t<VariableDecl> &parameter);

    size_t nr_params() const { return m_parameters.size(); }
    auto parameter_at(unsigned i) const { return m_parameters.at(i).get(); }

   private:
    std::vector<ptr_t<VariableDecl>> m_parameters {};
};