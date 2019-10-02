#include "CompilationUnit.hh"
#include "Function.hh"

void CompilationUnit::addFunction(ptr_t<Function> &function) {
    functions.push_back(std::move(function));
}

void CompilationUnit::functionPass(Scope *scope) const {
    for (auto &function : functions) {
        scope->addFunction(function->functionDecl.get());
    }
}

void CompilationUnit::analyze(Scope *scope) const {
    std::shared_ptr<Scope> innerScope = std::make_shared<Scope>(scope);

    for (auto &function : functions) {
        function->analyze(innerScope.get());
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