#include <fmt/format.h>

#include "FunctionDeclUser.hh"
#include "builtin/BuiltIn.hh"

void FunctionDeclUser::analyze(Scope *scope) {
    if (!scope->is_top_level()) {
        error::add_semantic_error(
            fmt::format("Nested function \"{}\" not allowed", identifier()), source_ref);
    }

    ptr_t<Scope> inner_scope = std::make_unique<Scope>(scope, this);

    m_parameter_list->analyze(inner_scope.get());
    m_body->analyze(inner_scope.get());
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