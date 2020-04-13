#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

#include "Util.hh"

enum class Primitive { STRING, INT, FLOAT, VOID, BOOL, DONT_CARE, INFERRED };
enum class Kind { PRIMITIVE, ARRAY, STRUCT, NONE };

class Operator;
class Expression;

namespace TypeUtil {

static std::unordered_map<std::string_view, Primitive> string_to_type {
    { "string", Primitive::STRING },
    { "int", Primitive::INT },
    { "float", Primitive::FLOAT },
    { "void", Primitive::VOID },
    { "bool", Primitive::BOOL }

};

Primitive from_string(const std::string_view s);

}  // namespace TypeUtil

class Type {
   public:
    Type() : m_primitive { Primitive::VOID }, m_kind { Kind::NONE } {}
    Type(const std::string_view s, Kind kind)
        : m_primitive { TypeUtil::from_string(s) }, m_kind { kind } {}

    Type(Primitive primitive)
        : m_primitive { primitive }, m_kind { Kind::PRIMITIVE } {}

    std::string dump(unsigned indent) const;

    bool operator==(const Type &other) const;
    bool operator!=(const Type &other) const;

    const auto &primitive() const { return m_primitive; }
    const auto &kind() const { return m_kind; }

    void change_primitive(const Primitive);
    void change_kind(const Kind);

    bool is_void() const { return m_primitive == Primitive::VOID; }

    std::string to_string() const;

   private:
    Primitive m_primitive;
    Kind m_kind;
};