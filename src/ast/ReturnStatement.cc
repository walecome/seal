#include "ReturnStatement.hh"

std::string ReturnStatement::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << Util::indent(indent) << name() << " (" << std::endl;
    oss << returnValue->dump(indent + 1) << std::endl;
    oss << Util::indent(indent) << ")";
    return oss.str();
}