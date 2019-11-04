#include "FloatLiteral.hh"

std::string FloatLiteral::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << value << "}";
    return oss.str();
}
