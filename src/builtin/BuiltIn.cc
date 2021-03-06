#include <functional>
#include <map>

#include "BuiltIn.hh"

namespace BuiltIn {

using builtin_func_t =
    std::function<ValueOperand(const std::vector<ValueOperand>&)>;

class FuncInfo {
   public:
    FuncInfo(const std::string&& identifier, const builtin_func_t func,
             const bool is_typechecked, const unsigned id)
        : m_identifier { std::move(identifier) },
          m_func { func },
          m_is_typechecked { is_typechecked },
          m_id { id } {}

    bool typechecked() const { return m_is_typechecked; }

    ValueOperand call(const std::vector<ValueOperand>& args) const {
        return m_func(args);
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
    BUILTIN_ENTRY(print, false), BUILTIN_ENTRY(println, false),
    // BUILTIN_ENTRY(input, false)
};

std::vector<FuncInfo> map_id_to_func() {
    std::vector<FuncInfo> ret {};

    for (auto it = builtin_functions.begin(); it != builtin_functions.end();
         ++it) {
        ret.push_back(it->second);
    }

    return ret;
}

static const std::vector<FuncInfo> id_to_builtin = map_id_to_func();

bool is_builtin(const std::string_view identifier) {
    return builtin_functions.find(std::string(identifier)) !=
           std::end(builtin_functions);
}

bool is_builtin(unsigned function_id) {
    return function_id < number_of_builtins();
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

ValueOperand call_builtin_function(unsigned function_id,
                                   const std::vector<ValueOperand>& args) {
    return id_to_builtin.at(function_id).call(args);
}

}  // namespace BuiltIn