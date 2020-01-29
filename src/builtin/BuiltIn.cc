#include <functional>
#include <map>

#include "BuiltIn.hh"

namespace BuiltIn {

using builtin_func_t = std::function<void(param_type_t)>;

class FuncInfo {
   public:
    FuncInfo(const std::string&& identifier, const builtin_func_t func,
             const bool is_typechecked, const unsigned id)
        : m_identifier { std::move(identifier) },
          m_func { func },
          m_is_typechecked { is_typechecked },
          m_id { id } {}

    bool typechecked() const { return m_is_typechecked; }

    template <typename... Args>
    void call(Args&&... args) const {
        m_func(std::forward<Args>(args)...);
    }

    unsigned id() const { return m_id; }

   private:
    const std::string m_identifier;
    const builtin_func_t m_func;
    const bool m_is_typechecked;
    const unsigned m_id;
};

unsigned get_and_increment_func_id() {
    static unsigned func_id = 0;
    return func_id++;
}

#define BUILTIN_ENTRY(func_ident, typechecked)          \
    {                                                   \
        (#func_ident), {                                \
            (#func_ident), (func_ident), (typechecked), \
                get_and_increment_func_id()             \
        }                                               \
    }

// @TODO: std::function sucks, replace with something else
// See following link for 3rd template argument
// https://stackoverflow.com/questions/35525777/use-of-string-view-for-map-lookup
static const std::map<std::string, FuncInfo, std::less<>> builtin_functions {
    BUILTIN_ENTRY(print, false), BUILTIN_ENTRY(println, false)
};

bool is_builtin(const std::string_view identifier) {
    return builtin_functions.find(std::string(identifier)) !=
           std::end(builtin_functions);
}

bool is_typechecked(const std::string_view identifier) {
    return builtin_functions.find(std::string(identifier))
        ->second.typechecked();
}

size_t number_of_builtins() { return builtin_functions.size(); }

unsigned function_id_from_identifier(std::string_view identifier) {
    ASSERT(is_builtin(identifier));
    return builtin_functions.find(identifier)->second.id();
}

void dispatch_builtin_function(const std::string_view identifier,
                               param_type_t args) {
    ASSERT(is_builtin(identifier));
    builtin_functions.find(identifier)->second.call(args);
}

}  // namespace BuiltIn