#pragma once

#include <map>
#include <string_view>
#include <variant>
#include <vector>

#include "Constants.hh"
#include "ast/ArgumentList.hh"
#include "ast/Type.hh"

#include "interpreter/Context.hh"
#include "interpreter/Value.hh"

class ArgumentList;

namespace BuiltIn {

using expr_value_t = std::variant<int, float, bool, std::string>;
using param_type_t = const std::vector<expr_value_t>&;

bool is_builtin(const std::string_view);
bool is_builtin(unsigned);
bool is_typechecked(const std::string_view);
Value call_builtin_function(unsigned, const std::vector<Value>&,
                            const Context& context);
size_t number_of_builtins();
unsigned function_id_from_identifier(std::string_view);

// Built-in functions
Value print(const std::vector<Value>&, const Context& context);
Value println(const std::vector<Value>&, const Context& context);
Value create_array(const std::vector<Value>&, const Context& context);
Value add_element(const std::vector<Value>&, const Context& context);
Value get_length(const std::vector<Value>&, const Context& context);
Value halt(const std::vector<Value>&, const Context& context);

}  // namespace BuiltIn
