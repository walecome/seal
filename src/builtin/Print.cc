#include <fmt/format.h>

#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"

namespace BuiltIn {

void print(const std::map<std::string_view, ValueOperand> &args) {
    std::vector<std::string> stringified_args {};

    for (auto &arg : args) {
        stringified_args.push_back(value_operand_to_string(arg.second));
    }

    fmt::print("{}", fmt::join(stringified_args, ""));
}

void println(const std::map<std::string_view, ValueOperand> &args) {
    print(args);
    fmt::print("\n");
}
}  // namespace BuiltIn
