#include <fmt/format.h>

#include "FunctionDeclUser.hh"
#include "builtin/BuiltIn.hh"

void FunctionDeclUser::analyze(Scope *scope) {
    FunctionDecl::analyze(scope);
    m_body->analyze(scope);
}

std::string FunctionDeclUser::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (id=" << m_id << ") "
        << "(" << std::endl;
    oss << util::indent(indent + 1) << "\"" << m_identifier << "\""
        << std::endl;
    if (m_parameter_list)
        oss << m_parameter_list->dump(indent + 1) << std::endl;
    else
        oss << util::indent(indent + 1) << "EMPTY PARAMETERLIST" << std::endl;
    oss << util::indent(indent + 1) << m_type.dump(0) << std::endl;
    oss << m_body->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";

    return oss.str();
}