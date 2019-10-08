#include "VariableExpression.hh"

void VariableExpression::analyze(Scope *scope) {
    if (!scope->has_variable(identifier.value, true)) {
        // @TODO: Sematic error instead
        error::add_semantic_error("Use of undeclared variable", identifier);
    }
}

std::string VariableExpression::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << identifier.value
        << "}";
    return oss.str();
}