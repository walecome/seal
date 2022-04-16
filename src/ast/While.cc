#include "ast/While.hh"

#include "ast/AstVisitor.hh"

void While::analyze(Scope *scope) {
    m_condition->analyze(scope);
    m_body->analyze(scope);
}

std::string While::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << m_condition->dump(indent + 1) << std::endl;
    oss << m_body->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";
    return oss.str();
}

void While::accept(const AstVisitor& visitor) {
  visitor.visit(*this);
}
