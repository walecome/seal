#include "Literal.hh"

std::string Literal::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << value.value << "}";
    return oss.str();
}