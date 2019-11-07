#include "CompilationUnit.hh"

void CompilationUnit::add_function(ptr_t<FunctionDecl> &function) {
    functions.push_back(std::move(function));
}

void CompilationUnit::function_pass(Scope *scope) {
    for (auto &function : functions) {
        scope->add_function(function.get());
    }
}

void CompilationUnit::analyze(Scope *scope) {
    for (auto &function : functions) {
        function->analyze(scope);
    }
}

std::string CompilationUnit::dump() const { return dump(0); }

std::string CompilationUnit::dump(unsigned indent) const {
    std::ostringstream oss {};
    for (auto &x : functions) {
        oss << "Dumping function: " << std::endl;
        oss << x->dump(indent) << std::endl;
    }
    return oss.str();
}