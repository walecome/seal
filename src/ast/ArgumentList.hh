#pragma once

#include <vector>

#include "Expression.hh"
#include "Node.hh"

class ArgumentList final : public Node {
    MAKE_DEFAULT_CONSTRUCTABLE(ArgumentList)
    MAKE_NONMOVABLE(ArgumentList)
    MAKE_NONCOPYABLE(ArgumentList)

    AST_NODE_NAME(ArgumentList)
    AST_DUMPABLE()
    AST_ANALYZABLE()

    void add_argument(ptr_t<Expression>& expression);

    size_t nr_args() const { return m_arguments.size(); }
    auto argument_at(unsigned i) const { return m_arguments.at(i).get(); }
    const auto& arguments() const { return m_arguments; }

    template <typename Function>
    void for_each_argument(Function callback) const {
        for (auto& argument : m_arguments) {
            callback(argument.get());
        }
    }

    template <typename Function>
    void for_each_enumerated_argument(Function callback) const {
        for (unsigned i = 0; i < m_arguments.size(); ++i) {
            callback(m_arguments[i].get(), i);
        }
    }

   private:
    std::vector<ptr_t<Expression>> m_arguments {};
};
