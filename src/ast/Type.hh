#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

#include "Util.hh"

enum class Primitive { STRING, INT, FLOAT, VOID, BOOL, DONT_CARE };
enum class Kind { PRIMITIVE, ARRAY, STRUCT, NONE };

struct Operator;
struct Expression;

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

struct Type {
    Type() : primitive { Primitive::VOID }, kind { Kind::NONE } {}
    Type(const std::string_view s, Kind kind)
        : primitive { TypeUtil::from_string(s) }, kind { kind } {}

    Type(Primitive primitive)
        : primitive { primitive }, kind { Kind::PRIMITIVE } {}

    std::string dump(unsigned indent) const;

    bool operator==(const Type &other) const;
    bool operator!=(const Type &other) const;

    std::string to_string() const;

    Primitive primitive;
    Kind kind;
};