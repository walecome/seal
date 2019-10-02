#include "VariableExpression.hh"

std::string VariableExpression::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << Util::indent(indent) << "{" << name() << ", " << identifier << "}";
    return oss.str();
}