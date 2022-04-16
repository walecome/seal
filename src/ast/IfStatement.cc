#include "ast/IfStatement.hh"

#include "ast/AstVisitor.hh"

void IfStatement::analyze(Scope* scope) {
    m_condition->analyze(scope);
    if (!m_condition->type().is_bool()) {
        error::add_semantic_error("Condition expression is not of type bool",
                                  source_ref);
    }
    m_if_body->analyze(scope);
    if (m_else_body) m_else_body->analyze(scope);
}

std::string IfStatement::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << m_condition->dump(indent + 1) << std::endl;
    oss << m_if_body->dump(indent + 1) << std::endl;
    if (m_else_body) {
        oss << m_else_body->dump(indent + 1) << std::endl;
    } else {
        oss << util::indent(indent + 1) << "NULL ELSEBODY" << std::endl;
    }
    oss << util::indent(indent) << ")";
    return oss.str();
}

void IfStatement::accept(const AstVisitor& visitor) {
  visitor.visit(*this);
}
