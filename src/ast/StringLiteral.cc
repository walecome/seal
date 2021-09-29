#include <iomanip>

#include "StringLiteral.hh"

std::string StringLiteral::dump(unsigned indent) const {
    return fmt::format("{} {{{}, {}}\n", util::indent(indent), name(), m_value.to_string());
}
