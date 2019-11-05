#include "IndexExpression.hh"

void IndexExpression::analyze(Scope* scope) {
    m_index->analyze(scope);
    if (m_index->type.kind != Kind::PRIMITIVE ||
        m_index->type.primitive != Primitive::INT) {
        error::add_semantic_error("Index type is not integer", source_ref);
    }

    m_indexed_expression->analyze(scope);

    type = m_indexed_expression->type;
    // @TODO: This needs to be extended to more complex types
    type.kind = Kind::PRIMITIVE;
}

std::string IndexExpression::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << m_indexed_expression->dump(indent + 1) << std::endl;
    oss << m_index->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";

    return oss.str();
}