#include "VariableDecl.hh"

std::string VariableDecl::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << Util::indent(indent) << name() << " (" << std::endl;
    oss << Util::indent(indent + 1) << "\"" << identifier << "\" : ";
    oss << type.dump(0) << std::endl;

    oss << value->dump(indent + 1) << std::endl;
    oss << Util::indent(indent) << ")";
    return oss.str();
}