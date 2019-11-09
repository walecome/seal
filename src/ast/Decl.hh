#pragma once

#include "Statement.hh"
// @TODO: Derive from something else
class Decl : public Statement {
    MAKE_NONMOVABLE(Decl)
    MAKE_NONCOPYABLE(Decl)
    AST_NODE_NAME(Decl)

   protected:
    Decl() = default;
};