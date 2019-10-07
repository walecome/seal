#include "IfStatement.hh"

void IfStatement::analyze(Scope* scope) const {
    condition->analyze(scope);
    if_body->analyze(scope);
    if (else_body) else_body->analyze(scope);
}

std::string IfStatement::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << condition->dump(indent + 1) << std::endl;
    oss << if_body->dump(indent + 1) << std::endl;
    if (else_body) {
        oss << else_body->dump(indent + 1) << std::endl;
    } else {
        oss << util::indent(indent + 1) << "NULL ELSEBODY" << std::endl;
    }
    oss << util::indent(indent) << ")";
    return oss.str();
}