#include "ast/VariableExpression.hh"

#include "ast/AstVisitor.hh"
#include "ast/VariableDecl.hh"

void VariableExpression::analyze(Scope *scope) {
    if (!scope->has_variable(m_identifier, true)) {
        error::add_semantic_error("Use of undeclared variable", source_ref);
        m_type = Type { Primitive::VOID };
    } else {
        m_type = scope->get_variable(m_identifier)->type();
    }
}

std::string VariableExpression::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << m_identifier << "}";
    return oss.str();
}

void VariableExpression::accept(const AstVisitor& visitor) {
  visitor.visit(*this);
}
