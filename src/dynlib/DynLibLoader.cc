#include "DynLibLoader.hh"
#include "DynLib.hh"

#include <dlfcn.h>
#include <fmt/format.h>
#include <functional>
#include <memory>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include "Constants.hh"

#define DECLARE_C_FUNC(name)  \
    void* (*lib_##name)(...); \
    *(void**)(&lib_##name) = dlsym(handle, #name);

namespace dynlib {

static std::unordered_map<std::string, ptr_t<DynamicLibrary>> s_loaded_libs {};

DynamicLibrary* get_lib(const std::string name, void* handle) {
    auto it = s_loaded_libs.find(name);
    if (it == s_loaded_libs.end()) {
        auto lib = std::make_unique<DynamicLibrary>(handle);
        DynamicLibrary* lib_ptr = lib.get();
        s_loaded_libs[name] = std::move(lib);
        return lib_ptr;
    }

    return it->second.get();
}

Result<DynamicLibrary*> load_lib(std::string_view filename, bool verbose) {
    std::string null_terminated_filename = std::string(filename);
    void* handle = dlopen(null_terminated_filename.c_str(), RTLD_LAZY);
    if (!handle) {
        if (verbose) {
            fmt::print("Failed to load shared library \"{}\": {}\n",
                       null_terminated_filename, dlerror());
        }
        return Result<DynamicLibrary*>::error();
    }

    // Clear existing errors
    dlerror();

    if (verbose) {
        fmt::print("Successfully loaded \"{}\"\n", null_terminated_filename);
    }

    return Result<DynamicLibrary*>::result(
        get_lib(null_terminated_filename, handle));
}

}  // namespace dynlib
