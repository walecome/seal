#include "Literal.hh"

#include "fmt/format.h"

std::string Literal::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << "}";

    return oss.str();
}