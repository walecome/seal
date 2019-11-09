#include "Expression.hh"

void Expression::set_primitive(const Primitive primitive) {
    m_type.change_primitive(primitive);
}
