#include "ast/RealLiteral.hh"

#include "ast/AstVisitor.hh"

std::string RealLiteral::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << m_value << "}";
    return oss.str();
}

void RealLiteral::accept(const AstVisitor& visitor) { visitor.visit(*this); }
