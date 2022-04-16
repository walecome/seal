#include "ast/ReturnStatement.hh"

#include "ast/AstVisitor.hh"
#include "ast/FunctionDecl.hh"

void ReturnStatement::analyze(Scope *scope) {
    m_return_value->analyze(scope);

    if (m_return_value->type() != scope->context->type()) {
        error::mismatched_type(scope->context->type(), m_return_value->type(),
                               source_ref);
    }
}

std::string ReturnStatement::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << m_return_value->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";
    return oss.str();
}

void ReturnStatement::accept(const AstVisitor& visitor) {
  visitor.visit(*this);
}
