#include "BinaryExpression.hh"
#include "Operator.hh"

void BinaryExpression::analyze(Scope *scope) {
    m_left->analyze(scope);
    m_operator->analyze(scope);
    m_right->analyze(scope);

    if (m_left->type().primitive() == Primitive::DONT_CARE &&
        m_right->type().primitive() == Primitive::DONT_CARE) {
        throw std::runtime_error("Can't deduce type of binary expression");
    } else if (m_left->type().primitive() == Primitive::DONT_CARE) {
        // @TODO: Should we only set the primitive and not the kind?

        m_left->set_primitive(m_right->type().primitive());
    } else if (m_right->type().primitive() == Primitive::DONT_CARE) {
        m_right->set_primitive(m_left->type().primitive());
    }

    if (m_left->type() == m_right->type()) {
        // @REFACTOR: Ugly logic
        if (this->type().primitive() != Primitive::VOID) return;
        m_type = m_left->type();

    } else {
        m_type = Type { Primitive::VOID };
        error::mismatched_type(m_left->type(), m_right->type(), source_ref);
    }
}

std::string BinaryExpression::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << m_left->dump(indent + 1) << std::endl;
    oss << m_operator->dump(indent + 1) << std::endl;
    oss << m_right->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";

    return oss.str();
}