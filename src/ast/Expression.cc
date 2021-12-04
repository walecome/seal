#include "Expression.hh"

void Expression::set_primitive(const Primitive primitive) {
    m_type.change_primitive(primitive);
}

void Expression::set_type(const Type type) {
    m_type = type;
}
