#pragma once

#include <string>
#include <unordered_map>

enum class Type { STRING, INT, FLOAT, VOID };

namespace TypeUtil {

static std::unordered_map<std::string, Type> stringToType {
    { "string", Type::STRING },
    { "int", Type::INT },
    { "float", Type::FLOAT },
    { "void", Type::VOID }
};

Type fromString(const std::string &s);
};  // namespace TypeUtil