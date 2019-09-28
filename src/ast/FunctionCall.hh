#pragma once

#include <vector>

#include "ArgumentList.hh"
#include "Expression.hh"

struct FunctionCall : public Expression {
    FunctionCall(const std::string identfier, ptr_t<ArgumentList> &argumentList)
        : identifier { identfier }, argumentList { std::move(argumentList) } {}

    virtual std::string name() const override { return "FunctionCall"; }
    virtual std::string dump() const override {
        std::ostringstream oss {};

        oss << name() << " (";
        oss << identifier << ", " << argumentList->dump() << ")";

        return oss.str();
    }

    const std::string identifier;
    ptr_t<ArgumentList> argumentList;
};