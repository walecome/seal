#include "Parameter.hh"

void Parameter::analyze(Scope *scope) const { scope->addVariable(this); }

std::string Parameter::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << Util::indent(indent) << name() << " ";
    oss << "\"" << identifier << "\": " << type.dump(indent);
    return oss.str();
}