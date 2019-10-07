#include "Type.hh"

Primitive TypeUtil::from_string(const std::string_view s) {
    auto it = string_to_type.find(s);

    if (it == std::end(string_to_type)) {
        throw 1;
    }

    return it->second;
}

std::string Type::dump(unsigned) const {
    std::ostringstream oss {};
    oss << "{Type, ";
    for (auto &x : TypeUtil::string_to_type) {
        if (x.second == primitive) {
            oss << x.first;
            break;
        }
    }
    oss << "}";
    return oss.str();
}