#include "CompilationUnit.hh"
#include "Function.hh"

void CompilationUnit::addFunction(ptr_t<Function> &function) {
    functions.push_back(std::move(function));
}