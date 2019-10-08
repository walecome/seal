#include "IntegerLiteral.hh"

std::string IntegerLiteral::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << value.value << "}";
    return oss.str();
}