#pragma once

#include <dlfcn.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "Constants.hh"
#include "Result.hh"

namespace dynlib {
class DynamicLibrary {
    using CFunctionType = void* (*)(...);

   public:
    DynamicLibrary(void* handle) : m_handle(handle) {}
    ~DynamicLibrary() { dlclose(m_handle); }

    class Callable {
    public:
        Callable() = default;

        template <class... Args>
        void call(Args... args) {
            m_func(args...);
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
        Callable callable =  Callable(f);
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