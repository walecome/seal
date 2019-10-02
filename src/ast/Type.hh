#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

#include "Util.hh"

enum class Primitive { STRING, INT, FLOAT, VOID };

namespace TypeUtil {

static std::unordered_map<std::string, Primitive> stringToType {
    { "string", Primitive::STRING },
    { "int", Primitive::INT },
    { "float", Primitive::FLOAT },
    { "void", Primitive::VOID }
};

Primitive fromString(const std::string &s);
}  // namespace TypeUtil

struct Type {
    Type(const std::string &s) : primitive { TypeUtil::fromString(s) } {}

    std::string dump(unsigned indent) const;

    Primitive primitive;
};