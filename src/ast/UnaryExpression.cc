#include "UnaryExpression.hh"
#include "VariableDecl.hh"
#include "VariableExpression.hh"

void UnaryExpression::analyze(Scope* scope) {
    m_expression->analyze(scope);
    m_operator->analyze(scope);
    m_type = m_expression->type();

    VariableExpression* variable_expr =
        dynamic_cast<VariableExpression*>(m_expression.get());

    if (variable_expr) {
        VariableDecl* variable_decl =
            scope->get_variable(variable_expr->identifier());

        if (!variable_decl->is_mutable()) {
            error::add_semantic_error(
                "Target variable of unary expression is not mutable",
                source_ref);
        }
    }
}

std::string UnaryExpression::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << m_operator->dump(indent + 1) << std::endl;
    oss << m_expression->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";

    return oss.str();
}
