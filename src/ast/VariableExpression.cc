#include "VariableExpression.hh"

void VariableExpression::analyze(Scope *scope) const {
    if (!scope->hasVariable(identifier)) {
        // @TODO: Sematic error instead
        throw std::runtime_error("Use of undeclared variable " +
                                 std::string(identifier));
    }
}

std::string VariableExpression::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << Util::indent(indent) << "{" << name() << ", " << identifier << "}";
    return oss.str();
}