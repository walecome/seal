#include <fmt/format.h>

#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"
#include "ir/IrFunction.hh"

namespace BuiltIn {

Value halt(const std::vector<Value>&, const Context& context) {
    fmt::print("Builtin halt() called!\n");
    exit(0);
}
}  // namespace BuiltIn
