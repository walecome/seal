#include <functional>
#include <unordered_map>

#include "BuiltIn.hh"

namespace BuiltIn {

using builtin_func_t = std::function<void(param_type_t)>;

class FuncInfo {
   public:
    FuncInfo(const std::string&& identifier, const builtin_func_t func,
             const bool is_typechecked)
        : m_identifier { std::move(identifier) },
          m_func { func },
          m_is_typechecked { is_typechecked } {}

    bool typechecked() const { return m_is_typechecked; }

    template <typename... Args>
    void call(Args&&... args) const {
        m_func(std::forward<Args>(args)...);
    }

   private:
    const std::string m_identifier;
    const builtin_func_t m_func;
    const bool m_is_typechecked;
};

#define BUILTIN_ENTRY(ident, func, typechecked)     \
    {                                               \
        (ident), { (ident), (func), (typechecked) } \
    }

// @TODO: std::function sucks, replace with something else
static const std::unordered_map<std::string, FuncInfo> BUILTIN_FUNCTIONS {
    BUILTIN_ENTRY("print", print, false),
    BUILTIN_ENTRY("println", println, false)
};

bool is_builtin(const std::string_view identifier) {
    return BUILTIN_FUNCTIONS.find(std::string(identifier)) !=
           std::end(BUILTIN_FUNCTIONS);
}

bool is_typechecked(const std::string_view identifier) {
    return BUILTIN_FUNCTIONS.find(std::string(identifier))
        ->second.typechecked();
}

void dispatch_builtin_function(const std::string_view identifier,
                               param_type_t args) {
    ASSERT(is_builtin(identifier));

    BUILTIN_FUNCTIONS.at(std::string(identifier)).call(args);
}

}  // namespace BuiltIn