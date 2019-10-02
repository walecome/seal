#include "Parameter.hh"

std::string Parameter::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << Util::indent(indent) << name() << " ";
    oss << "\"" << identifier << "\": " << type.dump(indent);
    return oss.str();
}