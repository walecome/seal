#include "VariableDecl.hh"

void VariableDecl::analyze(Scope *scope) {
    scope->add_variable(this);

    // Parameters don't have values
    if (!m_value) return;

    // Analyzing the value expression will resolve its type
    m_value->analyze(scope);

    if (m_type.primitive() == Primitive::INFERRED) {
        if (m_value->type().primitive() == Primitive::DONT_CARE) {
            // @TODO: Give good error message
            throw std::runtime_error("Cannot infer type");
        }

        m_type = m_value->type();
    }

    if (m_value->type().primitive() == Primitive::DONT_CARE) {
        m_value->set_primitive(m_type.primitive());
    }

    if (m_type != m_value->type()) {
        error::mismatched_type(m_type, m_value->type(), source_ref);
    }
}

std::string VariableDecl::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << util::indent(indent + 1) << "\"" << m_identifier << "\" : ";
    if (m_is_mutable) oss << "mutable ";
    oss << m_type.dump(0) << std::endl;

    if (m_value) oss << m_value->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";
    return oss.str();
}