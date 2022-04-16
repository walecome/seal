#pragma once

#include <vector>

#include "ArgumentList.hh"
#include "Expression.hh"
#include "FunctionDecl.hh"
#include "Token.hh"

class FunctionCall final : public Expression {
    MAKE_NONCOPYABLE(FunctionCall)
    MAKE_NONMOVABLE(FunctionCall)

    AST_NODE_NAME(FunctionCall)
    AST_DUMPABLE()
    AST_ANALYZABLE()

   public:
    FunctionCall(const std::string_view identifier,
                 ptr_t<ArgumentList>& argument_list)
        : m_identifier { identifier },
          m_argument_list { std::move(argument_list) } {}

    auto identifier() const { return m_identifier; }
    auto argument_list() const { return m_argument_list.get(); }

    auto declaration() const {
        ASSERT(m_decl != nullptr);
        return m_decl;
    }

    void accept(const AstVisitor& visitor) override;

   private:
    const std::string_view m_identifier;
    ptr_t<ArgumentList> m_argument_list;

    FunctionDecl* m_decl { nullptr };
};
