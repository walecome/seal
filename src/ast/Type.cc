#include "Type.hh"
#include "Expression.hh"
#include "Operator.hh"

Primitive TypeUtil::from_string(const std::string_view s) {
    auto it = string_to_type.find(s);
    assert(it != std::end(string_to_type));
    return it->second;
}

bool Type::operator==(const Type &other) const {
    return primitive == other.primitive && kind == other.kind;
}

bool Type::operator!=(const Type &other) const {
    return !this->operator==(other);
}

std::string Type::dump(unsigned) const {
    std::ostringstream oss {};
    oss << "{Type, ";
    oss << to_string();
    oss << "}";
    return oss.str();
}

std::string Type::to_string() const {
    for (auto &x : TypeUtil::string_to_type) {
        if (x.second == primitive) {
            return std::string(x.first);
        }
    }

    ASSERT_NOT_REACHED();
}