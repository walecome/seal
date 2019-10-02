#include "IfStatement.hh"

std::string IfStatement::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << Util::indent(indent) << name() << " (" << std::endl;
    oss << condition->dump(indent + 1) << std::endl;
    oss << ifBody->dump(indent + 1) << std::endl;
    if (elseBody) {
        oss << elseBody->dump(indent + 1) << std::endl;
    } else {
        oss << Util::indent(indent + 1) << "NULL ELSEBODY" << std::endl;
    }
    oss << Util::indent(indent) << ")";
    return oss.str();
}