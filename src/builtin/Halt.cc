#include <fmt/format.h>

#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"
#include "ir/IrFunction.hh"

namespace BuiltIn {

builtin_return_type_t halt(const builtin_args_t&) {
    fmt::print("Builtin halt() called!\n");
    exit(0);
}
}  // namespace BuiltIn
