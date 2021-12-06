#include <fmt/format.h>

#include "BuiltIn.hh"
#include "ir/IrFunction.hh"
#include "ast/ArgumentList.hh"

namespace BuiltIn {


ValueOperand halt(const std::vector<ValueOperand>&) {
    fmt::print("Builtin halt() called!\n");
    exit(0);
}
}
