#include "BooleanLiteral.hh"

std::string BooleanLiteral::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << value << "}";
    return oss.str();
}