#include "While.hh"

void While::analyze(Scope *scope) {
    condition->analyze(scope);
    body->analyze(scope);
}

std::string While::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << condition->dump(indent + 1) << std::endl;
    oss << body->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";
    return oss.str();
}