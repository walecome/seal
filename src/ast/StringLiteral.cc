#include "ast/StringLiteral.hh"

#include <iomanip>

#include "ast/AstVisitor.hh"

std::string StringLiteral::dump(unsigned indent) const {
    return fmt::format("{} {{{}, {}}}\n", util::indent(indent), name(),
                       m_value.to_string());
}

void StringLiteral::accept(const AstVisitor& visitor) { visitor.visit(*this); }
