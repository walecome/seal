#include "StringLiteral.hh"

std::string StringLiteral::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << value.value << "}";
    return oss.str();
}