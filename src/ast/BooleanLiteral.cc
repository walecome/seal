#include "ast/BooleanLiteral.hh"

#include "ast/AstVisitor.hh"

std::string BooleanLiteral::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << m_value << "}";
    return oss.str();
}


void BooleanLiteral::accept(const AstVisitor &visitor) { visitor.visit(*this); }
