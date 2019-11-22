#include "RealLiteral.hh"

std::string RealLiteral::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << m_value << "}";
    return oss.str();
}
