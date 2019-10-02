#include "FunctionDecl.hh"

std::string FunctionDecl::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << Util::indent(indent) << name() << "(" << std::endl;
    oss << Util::indent(indent + 1) << "\"" << identifier << "\"" << std::endl;
    if (parameterList)
        oss << parameterList->dump(indent + 1) << std::endl;
    else
        oss << Util::indent(indent + 1) << "EMPTY PARAMETERLIST" << std::endl;
    oss << Util::indent(indent) << ")";

    return oss.str();
}