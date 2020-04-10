#include "Util.hh"
#include <fmt/format.h>
#include <fstream>

namespace util {

std::string indent(unsigned steps) {
    const int indent_length = 4;
    return fmt::format("{:<{}}", "", indent_length * steps);
}

std::string indent_spaces(unsigned nr_spaces) {
    return fmt::format("{:<{}}", "", nr_spaces);
}

std::shared_ptr<std::string> read_source(const std::string &filename) {
    std::ifstream t(filename);
    return std::make_shared<std::string>((std::istreambuf_iterator<char>(t)),
                                         std::istreambuf_iterator<char>());
}

void print_col_indication(int col) {
    fmt::print("{:~<{}}", "", col - 1);
    fmt::print("^\n");
}

}  // namespace util