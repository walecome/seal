#pragma once

#include <dlfcn.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "ffi.h"

#include "CTypeWrapper.hh"
#include "Constants.hh"
#include "Result.hh"

namespace dynlib {
class DynamicLibrary {
    using CFunctionType = void (*)(void);

   public:
    DynamicLibrary(void* handle) : m_handle(handle) {}
    ~DynamicLibrary() { dlclose(m_handle); }

    class Callable {
       public:
        Callable() = default;

        void call() { throw 1; }

        template <std::size_t N>
        void call(vm::CResultWrapper& wrapper,
                  std::vector<ptr_t<vm::CTypeWrapper>>& values) {
            ffi_cif cif;
            ffi_type* arg_types[N];
            void* arg_values[N];

            for (std::size_t i = 0; i < N; ++i) {
                arg_types[i] = &values[i]->get_type();
                arg_values[i] = values[i]->get_value();
            }

            ffi_status status = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, N,
                                             &wrapper.type(), arg_types);
            if (status != FFI_OK) {
                throw 1;
            }

            ffi_call(&cif, FFI_FN(m_func), wrapper.buffer(), arg_values);
        }

       private:
        Callable(CFunctionType func) : m_func(func) {}

        CFunctionType m_func;

        friend DynamicLibrary;
    };

    Callable get_callable(const std::string& func) {
        auto it = m_cached_callables.find(func);
        if (it != m_cached_callables.end()) {
            return it->second;
        }

        CFunctionType f;
        *(void**)(&f) = dlsym(m_handle, func.c_str());
        Callable callable = Callable(f);
        m_cached_callables[func] = callable;
        return callable;
    }

    bool has_symbol(const std::string& func) {
        void* (*f)(...);
        *(void**)(&f) = dlsym(m_handle, func.c_str());
        return f != NULL;
    }

   private:
    void* const m_handle;

    std::unordered_map<std::string, Callable> m_cached_callables {};
};
}  // namespace dynlib
