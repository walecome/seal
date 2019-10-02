#include "Util.hh"

std::string Util::indent(unsigned steps) {
    std::ostringstream oss {};

    const std::string indentContent = "    ";

    for (unsigned i = 0; i < steps; ++i) {
        oss << indentContent;
    }

    return oss.str();
}