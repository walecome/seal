#pragma once

#include <vector>

#include "Expression.hh"
#include "Node.hh"

class ArgumentList : public Node {
    MAKE_DEFAULT_CONSTRUCTABLE(ArgumentList)
    MAKE_NONMOVABLE(ArgumentList)
    MAKE_NONCOPYABLE(ArgumentList)

    AST_NODE_NAME(ArgumentList)
    AST_DUMPABLE()
    AST_ANALYZABLE()

    void add_argument(ptr_t<Expression> &expression);
    const auto &arguments() const { return m_arguments; }

    template <typename Function>
    void for_each_argument(Function f) const;

   private:
    std::vector<ptr_t<Expression>> m_arguments {};
};
