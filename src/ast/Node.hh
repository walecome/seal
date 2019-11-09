#pragma once

#include <string>

#include "Constants.hh"
#include "Error.hh"
#include "Scope.hh"
#include "SourceRef.hh"
#include "Util.hh"

#define AST_NODE_NAME(node_name) \
   private:                      \
    virtual std::string name() const override { return #node_name; }

#define AST_DUMPABLE(node_name) \
   public:                      \
    virtual std::string dump(unsigned indent) const override;

#define AST_ANALYZABLE(node_name) \
   public:                        \
    virtual void analyze(Scope *scope) override;

class Node {
   public:
    virtual ~Node() = default;

    virtual void analyze(Scope *) {}

    virtual std::string name() const { return "Node"; }
    virtual std::string dump(unsigned) const { return "Node"; }

    SourceRef source_ref {};
};
