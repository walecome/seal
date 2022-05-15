#pragma once

#include <dlfcn.h>
#include <string>
#include <string_view>
#include <utility>

#include "Constants.hh"
#include "Result.hh"

namespace dynlib {
class DynamicLibrary;

Result<DynamicLibrary*> load_lib(std::string_view filename,
                                 bool verbose = false);

}  // namespace dynlib
