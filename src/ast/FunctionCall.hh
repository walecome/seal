#pragma once

#include <vector>

#include "ArgumentList.hh"
#include "Expression.hh"
#include "Token.hh"

class FunctionCall : public Expression {
    MAKE_NONCOPYABLE(FunctionCall)
    MAKE_NONMOVABLE(FunctionCall)

    AST_NODE_NAME(FunctionCall)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    FunctionCall(const Token identfier, ptr_t<ArgumentList>& argument_list)
        : m_identifier { identfier },
          m_argument_list { std::move(argument_list) } {}

    const auto& identifier() const { return m_identifier; }
    auto argument_list() const { return m_argument_list.get(); }

   private:
    const Token m_identifier;
    ptr_t<ArgumentList> m_argument_list;
};