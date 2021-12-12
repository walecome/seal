#include "ArrayLiteral.hh"

void ArrayLiteral::add_expression(ptr_t<Expression> &expression) {
    m_expressions.push_back(std::move(expression));
}

void ArrayLiteral::analyze(Scope *scope) {
    m_type.change_kind(Kind::ARRAY);

    if (m_expressions.empty()) {
        m_type.change_primitive(Primitive::DONT_CARE);
        return;
    }

    m_expressions.front()->analyze(scope);
    Type first = m_expressions.front()->type();

    for (unsigned i = 1; i < m_expressions.size(); ++i) {
        auto &current = m_expressions.at(i);
        current->analyze(scope);
        if (!current->is_literal()) {
            error::add_semantic_error("Array literals require literal values", source_ref);
        }
        if (current->type() != first) {
            throw std::runtime_error("Mismatched types in array literal");
        }
    }

    m_type.change_primitive(first.primitive());
}

std::string ArrayLiteral::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " [" << std::endl;

    for (auto &x : m_expressions) {
        oss << x->dump(indent + 1) << std::endl;
    }

    oss << util::indent(indent) << "]";

    return oss.str();
}