#pragma once

#include <string>

#include "Constants.hh"
#include "Scope.hh"
#include "Util.hh"

struct Node {
    virtual ~Node() = default;

    virtual void analyze(Scope *) const {}

    virtual std::string name() const { return "Node"; }
    virtual std::string dump(unsigned) const { return "Node"; }
};