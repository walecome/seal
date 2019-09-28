#pragma once

#include "Statement.hh"

#include <sstream>
#include <vector>

struct Block : public Statement {
    virtual std::string name() const override { return "Block"; }

    void addStatement(ptr_t<Statement> &statement);

    virtual std::string dump() const override {
        std::ostringstream oss {};
        oss << name() << " [";

        if (!statements.empty()) {
            for (unsigned i = 0; i < statements.size() - 1; ++i) {
                oss << statements[i]->dump() << ",";
            }

            oss << statements[statements.size() - 1]->dump();
        }
        oss << "]";
        return oss.str();
    }

    std::vector<ptr_t<Statement>> statements {};
};