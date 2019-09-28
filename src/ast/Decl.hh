#pragma once

#include "Statement.hh"
// @TODO: Derive from something else
struct Decl : public Statement {
    virtual std::string name() const override { return "Decl"; }
};