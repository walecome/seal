#pragma once

#include <dlfcn.h>
#include <string>
#include <utility>

#include "Constants.hh"
#include "Result.hh"

namespace dynlib {
class DynamicLibrary;

Result<ptr_t<DynamicLibrary>> load_lib(const std::string& filename,
                                       bool verbose = false);

class DynamicLibrary {
   public:
    DynamicLibrary(void* handle) : m_handle(handle) {}
    ~DynamicLibrary() { dlclose(m_handle); }

    template <class... Args>
    void call(const std::string& func, Args... args) {
        void* (*f)(...);
        *(void**)(&f) = dlsym(m_handle, func.c_str());
        f(args...);
    }

   private:
    void* const m_handle;
};
}  // namespace dynlib