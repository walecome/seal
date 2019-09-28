#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

enum class Primitive { STRING, INT, FLOAT, VOID };

namespace TypeUtil {

static std::unordered_map<std::string, Primitive> stringToType {
    { "string", Primitive::STRING },
    { "int", Primitive::INT },
    { "float", Primitive::FLOAT },
    { "void", Primitive::VOID }
};

Primitive fromString(const std::string &s);
};  // namespace TypeUtil

struct Type {
    Type(const std::string &s) : primitive { TypeUtil::fromString(s) } {}

    std::string dump() const {
        std::ostringstream oss {};
        oss << "{Type, ";
        for (auto &x : TypeUtil::stringToType) {
            if (x.second == primitive) {
                oss << x.first;
                break;
            }
        }
        oss << "}";
        return oss.str();
    }

    Primitive primitive;
};