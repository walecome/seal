#pragma once

#include "Literal.hh"

struct ArrayLiteral : public Literal {
    void addExpression(ptr_t<Expression> &expression);

    virtual std::string name() const override { return "ArrayLiteral"; }
    virtual std::string dump() const override {
        std::ostringstream oss {};
        oss << name() << " [";

        if (!expressions.empty()) {
            for (unsigned i = 0; i < expressions.size() - 1; ++i) {
                oss << expressions[i]->dump() << ",";
            }

            oss << expressions[expressions.size() - 1]->dump();
        }
        oss << "]";

        return oss.str();
    }

    std::vector<ptr_t<Expression>> expressions {};
};