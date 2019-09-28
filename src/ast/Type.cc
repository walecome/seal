#include "Type.hh"

Primitive TypeUtil::fromString(const std::string &s) {
    auto it = stringToType.find(s);

    if (it == std::end(stringToType)) {
        throw 1;
    }

    return it->second;
}