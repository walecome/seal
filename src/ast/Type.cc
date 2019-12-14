#include "Type.hh"
#include "Expression.hh"
#include "Operator.hh"

Primitive TypeUtil::from_string(const std::string_view s) {
    auto it = string_to_type.find(s);
    assert(it != std::end(string_to_type));
    return it->second;
}

bool Type::operator==(const Type &other) const {
    return m_primitive == other.m_primitive && m_kind == other.m_kind;
}

bool Type::operator!=(const Type &other) const {
    return !this->operator==(other);
}

void Type::change_kind(const Kind new_kind) {
    // @TODO: We should be able to assert this when we add support for more
    //        complex types
    // ASSERT(m_kind == Kind::NONE);
    m_kind = new_kind;
}
void Type::change_primitive(const Primitive new_primitive) {
    ASSERT(m_primitive == Primitive::VOID ||
           m_primitive == Primitive::DONT_CARE);
    m_primitive = new_primitive;
}

std::string Type::dump(unsigned) const {
    std::ostringstream oss {};
    oss << "{Type, ";
    if (m_kind == Kind::ARRAY) oss << "[";
    oss << to_string();
    if (m_kind == Kind::ARRAY) oss << "]";
    oss << "}";
    return oss.str();
}

std::string Type::to_string() const {
    for (auto &x : TypeUtil::string_to_type) {
        if (x.second == m_primitive) {
            return std::string(x.first);
        }
    }

    if (m_primitive == Primitive::INFERRED) {
        return "INFERRED";
    }

    ASSERT_NOT_REACHED();
}