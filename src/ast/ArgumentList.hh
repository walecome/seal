#pragma once

#include <vector>

#include "Expression.hh"
#include "Node.hh"

struct ArgumentList : public Node {
    virtual std::string name() const override { return "ArgumentList"; }

    virtual std::string dump() const override {
        std::ostringstream oss {};

        oss << name() << " [";

        if (!arguments.empty()) {
            for (unsigned i = 0; i < arguments.size() - 1; ++i) {
                oss << arguments[i]->dump() << ",";
            }

            oss << arguments[arguments.size() - 1]->dump();
        }
        oss << "]";

        return oss.str();
    }

    void addArgument(ptr_t<Expression> &expression);

    std::vector<ptr_t<Expression>> arguments {};
};