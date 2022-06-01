#pragma once

#include <map>
#include <string_view>
#include <variant>
#include <vector>

#include "Constants.hh"
#include "ast/ArgumentList.hh"
#include "ast/Type.hh"

class ArgumentList;
class Context;
class ValueFactory;

namespace BuiltIn {

using builtin_arg_t = ptr_t<ValueFactory>;
using builtin_return_type_t = ptr_t<ValueFactory>;
using builtin_args_t = std::vector<builtin_arg_t>;

bool is_builtin(const std::string_view);
bool is_builtin(unsigned);
bool is_typechecked(const std::string_view);
builtin_return_type_t call_builtin_function(unsigned, const builtin_args_t&);
size_t number_of_builtins();
unsigned function_id_from_identifier(std::string_view);

// Built-in functions
builtin_return_type_t print(const builtin_args_t&);
builtin_return_type_t println(const builtin_args_t&);
builtin_return_type_t create_array(const builtin_args_t&);
builtin_return_type_t add_element(const builtin_args_t&);
builtin_return_type_t get_length(const builtin_args_t&);
builtin_return_type_t halt(const builtin_args_t&);

}  // namespace BuiltIn
