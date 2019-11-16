#include "Util.hh"

namespace util {

std::string indent(unsigned steps) {
    std::ostringstream oss {};

    const std::string indent_content = "    ";

    for (unsigned i = 0; i < steps; ++i) {
        oss << indent_content;
    }

    return oss.str();
}

std::string indent_spaces(unsigned nr_spaces) {
    std::ostringstream oss {};

    for (unsigned i = 0; i < nr_spaces; ++i) {
        oss << " ";
    }

    return oss.str();
}

std::shared_ptr<std::string> read_source(const std::string &filename) {
    std::ifstream t(filename);
    return std::make_shared<std::string>((std::istreambuf_iterator<char>(t)),
                                         std::istreambuf_iterator<char>());
}

void print_col_indication(int col) {
    for (int i = 0; i < col; ++i) {
        std::cout << "~";
    }

    std::cout << "^" << std::endl;
}

}  // namespace util