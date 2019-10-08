#pragma once

#include <vector>

#include "ArgumentList.hh"
#include "Expression.hh"

struct FunctionCall : public Expression {
    FunctionCall(const std::string_view identfier,
                 ptr_t<ArgumentList> &argument_list)
        : identifier { identfier },
          argument_list { std::move(argument_list) } {}

    virtual std::string name() const override { return "FunctionCall"; }
    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) override;

    const std::string_view identifier;
    ptr_t<ArgumentList> argument_list;
};