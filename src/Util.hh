#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <streambuf>
#include <string_view>

namespace Util {
std::string indent(unsigned steps);
std::shared_ptr<std::string> readSource(const std::string &filenames);
}  // namespace Util