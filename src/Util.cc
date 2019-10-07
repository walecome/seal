#include "Util.hh"

std::string util::indent(unsigned steps) {
    std::ostringstream oss {};

    const std::string indent_content = "    ";

    for (unsigned i = 0; i < steps; ++i) {
        oss << indent_content;
    }

    return oss.str();
}

std::shared_ptr<std::string> util::read_source(const std::string &filename) {
    std::ifstream t(filename);
    return std::make_shared<std::string>((std::istreambuf_iterator<char>(t)),
                                         std::istreambuf_iterator<char>());
}