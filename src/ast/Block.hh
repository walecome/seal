#pragma once

#include "Statement.hh"

#include <sstream>
#include <vector>

struct Block : public Statement {
    virtual std::string name() const override { return "Block"; }

    void add_statement(ptr_t<Statement> &statement);

    virtual std::string dump(unsigned indent) const override;

    virtual void analyze(Scope *scope) const override;

    std::vector<ptr_t<Statement>> statements {};
};