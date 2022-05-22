#include <iomanip>

#include <fmt/format.h>

#include "StringLiteral.hh"

std::string StringLiteral::dump(unsigned indent) const {
    return fmt::format("{} {{{}, {}}}\n", util::indent(indent), name(), m_value);
}
