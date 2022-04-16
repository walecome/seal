#include "ast/CompilationUnit.hh"

#include "ast/AstVisitor.hh"

void CompilationUnit::add_function(ptr_t<FunctionDecl> &function) {
    m_functions.push_back(std::move(function));
}

void CompilationUnit::function_pass(Scope *scope) {
    for (auto &function : m_functions) {
        scope->add_function(function.get());
    }
}

void CompilationUnit::analyze(Scope *scope) {
    for (auto &function : m_functions) {
        function->analyze(scope);
    }
}

std::string CompilationUnit::dump() const { return dump(0); }

std::string CompilationUnit::dump(unsigned indent) const {
    std::ostringstream oss {};
    for (auto &x : m_functions) {
        oss << "Dumping function: " << std::endl;
        oss << x->dump(indent) << std::endl;
    }
    return oss.str();
}

void CompilationUnit::accept(const AstVisitor& visitor) {
  visitor.visit(*this);
}

