#include <fmt/format.h>

#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"
#include "ir/IrFunction.hh"

namespace BuiltIn {

ValueOperand print(const std::vector<ValueOperand> &args) {
    std::vector<std::string> stringified_args {};

    for (auto &arg : args) {
        stringified_args.push_back(arg.to_value_string());
    }

    std::string s = fmt::format("{}", fmt::join(stringified_args, ""));
    fmt::print("{}", s);

    return create_value_operand<IntOperand>(s.size());
}

ValueOperand println(const std::vector<ValueOperand> &args) {
    unsigned long chars_printed = print(args).as_int();
    fmt::print("\n");
    return create_value_operand<IntOperand>(chars_printed + 1);
}
}  // namespace BuiltIn
