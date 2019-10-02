#pragma once

#include <string>

#include "Constants.hh"
#include "Util.hh"

struct Scope;

struct Node {
    virtual ~Node() = default;

    virtual void analyze(const Scope &scope) const {}

    virtual std::string name() const { return "Node"; }
    virtual std::string dump(unsigned indent) const { return "Node"; }
};