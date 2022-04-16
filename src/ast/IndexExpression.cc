#include "ast/IndexExpression.hh"

#include "ast/AstVisitor.hh"

void IndexExpression::analyze(Scope* scope) {
    m_index->analyze(scope);
    if (m_index->type().kind() != Kind::PRIMITIVE ||
        m_index->type().primitive() != Primitive::INT) {
        error::add_semantic_error("Index type is not integer", source_ref);
    }

    m_indexed_expression->analyze(scope);

    m_type = m_indexed_expression->type();

    if (m_type.kind() != Kind::ARRAY &&
        (m_type.kind() == Kind::PRIMITIVE && !m_type.is_string())) {
        error::add_semantic_error(
            "Indexed expression needs to be of array or string type",
            source_ref);
    }

    // @TODO: This needs to be extended to more complex types
    m_type.change_kind(Kind::PRIMITIVE);
}

std::string IndexExpression::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << m_indexed_expression->dump(indent + 1) << std::endl;
    oss << m_index->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";

    return oss.str();
}

void IndexExpression::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}
