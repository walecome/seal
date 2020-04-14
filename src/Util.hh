#pragma once

#include <memory>
#include <string_view>

class TokenBuffer;

namespace util {
std::string indent(unsigned steps);
std::string indent_spaces(unsigned nr_spaces);
void print_col_indication(int col);
std::shared_ptr<std::string> read_source(const std::string &filename);

}  // namespace util