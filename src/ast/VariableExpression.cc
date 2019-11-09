#include "VariableExpression.hh"
#include "VariableDecl.hh"

void VariableExpression::analyze(Scope *scope) {
    if (!scope->has_variable(m_identifier.value, true)) {
        // @TODO: Sematic error instead
        error::add_semantic_error("Use of undeclared variable", source_ref);
        m_type = Type { Primitive::VOID };
    } else {
        m_type = scope->get_variable(m_identifier.value)->type();
    }
}

std::string VariableExpression::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << m_identifier.value
        << "}";
    return oss.str();
}