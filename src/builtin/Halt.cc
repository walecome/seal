#include <fmt/format.h>

#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"
#include "ir/IrFunction.hh"

namespace BuiltIn {

PoolEntry halt(const std::vector<PoolEntry>&, const Context&) {
    fmt::print("Builtin halt() called!\n");
    exit(0);
}
}  // namespace BuiltIn
