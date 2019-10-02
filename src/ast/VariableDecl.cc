#include "VariableDecl.hh"

void VariableDecl::analyze(Scope *scope) const {
    // @TODO: Type check
}

std::string VariableDecl::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << Util::indent(indent) << name() << " (" << std::endl;
    oss << Util::indent(indent + 1) << "\"" << identifier << "\" : ";
    oss << type.dump(0) << std::endl;

    if (value) oss << value->dump(indent + 1) << std::endl;
    oss << Util::indent(indent) << ")";
    return oss.str();
}