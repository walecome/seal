#include "Function.hh"

void Function::analyze(Scope* scope) {
    ptr_t<Scope> inner_scope = std::make_unique<Scope>(scope);

    function_decl->analyze(inner_scope.get());
    block->analyze(inner_scope.get());
}

std::string Function::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << "(" << std::endl;
    oss << function_decl->dump(indent + 1) << std::endl;
    oss << block->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";
    return oss.str();
}