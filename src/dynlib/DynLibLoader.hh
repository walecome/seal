#pragma once

#include <dlfcn.h>
#include <string>
#include <utility>

#include "Constants.hh"
#include "Result.hh"

namespace dynlib {
class DynamicLibrary;

Result<DynamicLibrary*> load_lib(const std::string& filename,
                                 bool verbose = false);

}  // namespace dynlib