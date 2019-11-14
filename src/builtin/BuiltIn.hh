#pragma once

#include <string_view>
#include <variant>
#include <vector>

#include "Constants.hh"
#include "ast/ArgumentList.hh"
#include "ast/Type.hh"

class ArgumentList;

namespace BuiltIn {

using expr_value_t = std::variant<int, float, bool, std::string>;
using param_type_t = const std::vector<expr_value_t>&;

bool is_builtin(const std::string_view);
bool is_typechecked(const std::string_view);
void dispatch_builtin_function(const std::string_view, param_type_t);

// Built-in functions
void print(param_type_t args);
void println(param_type_t args);
void __assert(param_type_t args);

}  // namespace BuiltIn
