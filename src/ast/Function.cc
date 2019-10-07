#include "Function.hh"

void Function::analyze(Scope* scope) const {
    function_decl->analyze(scope);
    block->analyze(scope);
}

std::string Function::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << "(" << std::endl;
    oss << function_decl->dump(indent + 1) << std::endl;
    oss << block->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";
    return oss.str();
}