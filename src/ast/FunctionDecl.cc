#include "FunctionDecl.hh"

void FunctionDecl::analyze(Scope *scope) {
    if (!scope->is_top_level()) {
        scope->add_function(this);
    }

    ptr_t<Scope> inner_scope = std::make_unique<Scope>(scope, this);
    m_parameter_list->analyze(inner_scope.get());
    m_body->analyze(inner_scope.get());
}

std::string FunctionDecl::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << "(" << std::endl;
    oss << util::indent(indent + 1) << "\"" << m_identifier.value << "\""
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