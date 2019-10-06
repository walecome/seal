#include "Util.hh"

std::string Util::indent(unsigned steps) {
    std::ostringstream oss {};

    const std::string indentContent = "    ";

    for (unsigned i = 0; i < steps; ++i) {
        oss << indentContent;
    }

    return oss.str();
}

std::shared_ptr<std::string> Util::readSource(const std::string &filename) {
    std::ifstream t(filename);
    return std::make_shared<std::string>((std::istreambuf_iterator<char>(t)),
                                         std::istreambuf_iterator<char>());
}