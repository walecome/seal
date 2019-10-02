#include "Function.hh"

void Function::analyze(Scope* scope) const {
    functionDecl->analyze(scope);
    block->analyze(scope);
}

std::string Function::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << Util::indent(indent) << name() << "(" << std::endl;
    oss << functionDecl->dump(indent + 1) << std::endl;
    oss << block->dump(indent + 1) << std::endl;
    oss << Util::indent(indent) << ")";
    return oss.str();
}