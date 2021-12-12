#include "ast/StructDecl.hh"

#include <sstream>

std::string StructDecl::dump(unsigned int indent) const {
    std::stringstream ss {};

    ss << util::indent(indent) << name() << "{" << std::endl;
    ss << util::indent(indent + 1) << m_identifier << std::endl;
    for (auto& member_func : m_member_functions) {
        ss << member_func->dump(indent + 1) << std::endl;
    }

    for (auto& member : m_members) {
        ss << member->dump(indent + 1) << std::endl;
    }

    ss << util::indent(indent) << "}";

    return ss.str();
}
