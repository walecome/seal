#pragma once

#include "Function.hh"
#include "Node.hh"

#include <vector>

struct CompilationUnit : public Node {
    void addFunction(ptr_t<Function> &function);
    std::vector<ptr_t<Function>> functions {};
};