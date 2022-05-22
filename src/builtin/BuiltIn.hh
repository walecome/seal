#pragma once

#include <map>
#include <string_view>
#include <variant>
#include <vector>

#include "Constants.hh"
#include "ast/ArgumentList.hh"
#include "ast/Type.hh"

#include "common/PoolEntry.hh"

class ArgumentList;
class Context;

namespace BuiltIn {

using expr_value_t = std::variant<int, float, bool, std::string>;
using param_type_t = const std::vector<expr_value_t>&;

bool is_builtin(const std::string_view);
bool is_builtin(unsigned);
bool is_typechecked(const std::string_view);
PoolEntry call_builtin_function(unsigned, const std::vector<PoolEntry>&,
                                Context& context);
size_t number_of_builtins();
unsigned function_id_from_identifier(std::string_view);

// Built-in functions
PoolEntry print(const std::vector<PoolEntry>&, Context& context);
PoolEntry println(const std::vector<PoolEntry>&, Context& context);
PoolEntry create_array(const std::vector<PoolEntry>&, Context& context);
PoolEntry add_element(const std::vector<PoolEntry>&, Context& context);
PoolEntry get_length(const std::vector<PoolEntry>&, Context& context);
PoolEntry halt(const std::vector<PoolEntry>&, Context& context);

}  // namespace BuiltIn
