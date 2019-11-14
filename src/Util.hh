#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <streambuf>
#include <string_view>

namespace util {
std::string indent(unsigned steps);
std::string indent_spaces(unsigned nr_spaces);

std::shared_ptr<std::string> read_source(const std::string &filename);
}  // namespace util