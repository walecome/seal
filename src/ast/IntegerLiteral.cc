#include "ast/IntegerLiteral.hh"

#include "ast/AstVisitor.hh"

std::string IntegerLiteral::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", " << m_value << "}";
    return oss.str();
}

void IntegerLiteral::accept(const AstVisitor& visitor) { visitor.visit(*this); }
