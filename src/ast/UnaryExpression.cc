#include "UnaryExpression.hh"

void UnaryExpression::analyze(Scope *scope) {
    m_expression->analyze(scope);
    m_operator->analyze(scope);

    m_type = m_expression->type();
}

std::string UnaryExpression::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << m_operator->dump(indent + 1) << std::endl;
    oss << m_expression->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";

    return oss.str();
}
