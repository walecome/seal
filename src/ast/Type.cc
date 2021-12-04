#include <fmt/format.h>

#include "Type.hh"
#include "Expression.hh"
#include "Operator.hh"

namespace {
std::string to_debug_string(Primitive primitive) {
    switch (primitive) {
        case Primitive::STRING:
            return "STRING";
        case Primitive::INT:
            return "INT";
        case Primitive::FLOAT:
            return "FLOAT";
        case Primitive::VOID:
            return "VOID";
        case Primitive::BOOL:
            return "BOOL";
        case Primitive::DONT_CARE:
            return "DONT_CARE";
        case Primitive::INFERRED:
            return "INFERRED";
    }
    ASSERT_NOT_REACHED();
}

std::string to_debug_string(Kind kind) {
    switch (kind) {
        case Kind::PRIMITIVE:
            return "PRIMITIVE";
        case Kind::ARRAY:
            return "ARRAY";
        case Kind::STRUCT:
            return "STRUCT";
        case Kind::NONE:
            return "NONE";
    }
    ASSERT_NOT_REACHED();
}
}

Primitive TypeUtil::from_string(const std::string_view s) {
    auto it = string_to_type.find(s);
    assert(it != std::end(string_to_type));
    return it->second;
}

bool Type::operator==(const Type &other) const {
    bool check_kind = m_primitive != Primitive::DONT_CARE && other.m_primitive != Primitive::DONT_CARE;

    if (m_primitive != other.m_primitive) {
        return false;
    }

    if (check_kind) {
        return m_kind == other.m_kind;
    }

    return true;
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
    return fmt::format("{{ Primitive::{}, Kind::{} }}", to_debug_string(m_primitive), to_debug_string(m_kind));
}

std::string Type::to_user_string() const {
    switch (m_primitive) {
        case Primitive::STRING:
            return "string";
        case Primitive::INT:
            return "int";
        case Primitive::FLOAT:
            return "float";
        case Primitive::BOOL:
            return "bool";
        default:
            ASSERT_NOT_REACHED();
    }
    return "";
}
