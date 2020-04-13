#include <fmt/format.h>

#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"

namespace BuiltIn {

void print(const std::vector<ValueOperand> &args) {
    std::vector<std::string> stringified_args {};

    for (auto &arg : args) {
        stringified_args.push_back(value_operand_to_string(arg));
    }

    fmt::print("{}", fmt::join(stringified_args, ""));
}

void println(const std::vector<ValueOperand> &args) {
    print(args);
    fmt::print("\n");
}
}  // namespace BuiltIn
