#pragma once

#include "Node.hh"
#include "VariableDecl.hh"

#include <vector>

class ParameterList final : public Node {
    MAKE_DEFAULT_CONSTRUCTABLE(ParameterList)
    MAKE_NONMOVABLE(ParameterList)
    MAKE_NONCOPYABLE(ParameterList)

    AST_NODE_NAME(ParameterList)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    void add_parameter(ptr_t<VariableDecl>& parameter);

    size_t nr_params() const { return m_parameters.size(); }
    auto parameter_at(unsigned i) const { return m_parameters.at(i).get(); }

    template <class Function>
    void for_each_parameter(Function callback) const {
        for (auto& param : m_parameters) {
            callback(param.get());
        }
    }

    template <class Function>
    void enumerate_paramters(Function callback) const {
        for (size_t i = 0; i < m_parameters.size(); ++i) {
            callback(i, m_parameters.at(i));
        }
    }

   private:
    std::vector<ptr_t<VariableDecl>> m_parameters {};
};
