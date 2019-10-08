#include "VariableExpression.hh"

void VariableExpression::analyze(Scope *scope) {
    if (!scope->has_variable(identifier)) {
        // @TODO: Sematic error instead
        error::add_semantic_error("Use of undeclared variable " +
                                  std::string(identifier));
    }
}

std::string VariableExpression::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << identifier << "}";
    return oss.str();
}