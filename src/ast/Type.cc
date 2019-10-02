#include "Type.hh"

Primitive TypeUtil::fromString(const std::string &s) {
    auto it = stringToType.find(s);

    if (it == std::end(stringToType)) {
        throw 1;
    }

    return it->second;
}

std::string Type::dump(unsigned indent) const {
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