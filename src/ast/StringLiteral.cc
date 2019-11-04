#include <iomanip>

#include "StringLiteral.hh"

std::string StringLiteral::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << std::quoted(value)
        << "}";
    return oss.str();
}