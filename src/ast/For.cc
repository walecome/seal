#include "ast/For.hh"

#include "ast/AstVisitor.hh"

void For::analyze(Scope *scope) {
    auto inner_scope = std::make_unique<Scope>(scope);

    m_initial_expression->analyze(inner_scope.get());
    m_stop_condition->analyze(inner_scope.get());
    m_per_iteration_expression->analyze(inner_scope.get());
    m_body->analyze(inner_scope.get());
}

std::string For::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << m_initial_expression->dump(indent + 1) << std::endl;
    oss << m_stop_condition->dump(indent + 1) << std::endl;
    oss << m_per_iteration_expression->dump(indent + 1) << std::endl;
    oss << m_body->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";
    return oss.str();
}

void For::accept(const AstVisitor& visitor) {
  visitor.visit(*this);
}
