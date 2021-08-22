#include "DynLibLoader.hh"

#include <dlfcn.h>
#include <fmt/format.h>
#include <functional>
#include <memory>
#include "Constants.hh"

#define DECLARE_C_FUNC(name)  \
    void* (*lib_##name)(...); \
    *(void**)(&lib_##name) = dlsym(handle, #name);

namespace dynlib {
Result<ptr_t<DynamicLibrary>> load_lib(const std::string& filename, bool verbose) {
    void* handle = dlopen(filename.c_str(), RTLD_LAZY);
    if (!handle) {
      if (verbose) {
        fmt::print("Failed to load shared library \"{}\": {}\n", filename,
                   dlerror());
      }
      return Result<ptr_t<DynamicLibrary>>::error();
    }

    // Clear existing errors
    dlerror();

    if (verbose) {
      fmt::print("Successfully loaded \"{}\"\n", filename);
    } 

    ptr_t<DynamicLibrary> lib = std::make_unique<DynamicLibrary>(handle);
    return Result<ptr_t<DynamicLibrary>>::result(std::move(lib));
}


}  // namespace dynlib