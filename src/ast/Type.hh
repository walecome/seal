#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

#include "Util.hh"

enum class Primitive { STRING, INT, FLOAT, VOID };

namespace TypeUtil {

static std::unordered_map<std::string_view, Primitive> string_to_type {
    { "string", Primitive::STRING },
    { "int", Primitive::INT },
    { "float", Primitive::FLOAT },
    { "void", Primitive::VOID }
};

Primitive from_string(const std::string_view s);
}  // namespace TypeUtil

struct Type {
    Type(const std::string_view s) : primitive { TypeUtil::from_string(s) } {}

    std::string dump(unsigned indent) const;

    Primitive primitive;
};