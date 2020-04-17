#pragma once

#include <map>
#include <string_view>
#include <variant>
#include <vector>

#include "Constants.hh"
#include "ast/ArgumentList.hh"
#include "ast/Type.hh"

#include "ir/Operand.hh"

class ArgumentList;

namespace BuiltIn {

using expr_value_t = std::variant<int, float, bool, std::string>;
using param_type_t = const std::vector<expr_value_t>&;

bool is_builtin(const std::string_view);
bool is_builtin(unsigned);
bool is_typechecked(const std::string_view);
ValueOperand call_builtin_function(unsigned, const std::vector<ValueOperand>&);
size_t number_of_builtins();
unsigned function_id_from_identifier(std::string_view);

// Built-in functions
ValueOperand print(const std::vector<ValueOperand>&);
ValueOperand println(const std::vector<ValueOperand>&);
ValueOperand input(const std::vector<ValueOperand>&);

}  // namespace BuiltIn
