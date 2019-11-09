#include "VariableDecl.hh"

void VariableDecl::analyze(Scope *scope) {
    scope->add_variable(this);

    // Parameters don't have values
    if (!m_value) return;

    // Analyzing the value expression will resolve its type
    m_value->analyze(scope);

    if (m_value->type().primitive() == Primitive::DONT_CARE)
        m_value->set_primitive(m_type.primitive());

    if (m_type != m_value->type()) {
        error::mismatched_type(m_type, m_value->type(), source_ref);
    }
}

std::string VariableDecl::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << util::indent(indent + 1) << "\"" << m_identifier.value << "\" : ";
    oss << m_type.dump(0) << std::endl;

    if (m_value) oss << m_value->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";
    return oss.str();
}