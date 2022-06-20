#pragma once

#include <map>
#include <string_view>
#include <variant>
#include <vector>

#include "Constants.hh"
#include "ast/ArgumentList.hh"
#include "ast/Type.hh"

#include "common/BuiltinFunctionAddress.hh"
#include "common/Value.hh"

class ArgumentList;
class Context;

namespace BuiltIn {

using builtin_arg_t = Value;
using builtin_return_type_t = Value;
using builtin_args_t = std::vector<Value>;

bool is_builtin(const std::string_view);
bool is_typechecked(const std::string_view);
builtin_return_type_t call_builtin_function(BuiltinFunctionAddress func, const builtin_args_t&);
unsigned function_id_from_identifier(std::string_view);

// Built-in functions
builtin_return_type_t print(const builtin_args_t&);
builtin_return_type_t println(const builtin_args_t&);
builtin_return_type_t create_array(const builtin_args_t&);
builtin_return_type_t add_element(const builtin_args_t&);
builtin_return_type_t get_length(const builtin_args_t&);
builtin_return_type_t halt(const builtin_args_t&);

}  // namespace BuiltIn
