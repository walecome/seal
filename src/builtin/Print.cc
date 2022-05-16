#include <fmt/format.h>

#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"
#include "ir/IrFunction.hh"

namespace BuiltIn {

Value print(const std::vector<Value> &args, const Context &context) {
    std::vector<std::string> stringified_args {};

    for (auto &arg : args) {
        stringified_args.push_back(arg.to_string());
    }

    std::string s = fmt::format("{}", fmt::join(stringified_args, ""));
    fmt::print("{}", s);

    return context.create_value_from(Integer(s.size()));
}

Value println(const std::vector<Value> &args, const Context &context) {
    unsigned long chars_printed = print(args, context).as_integer().value();
    fmt::print("\n");
    return context.create_value_from(Integer(chars_printed + 1));
}
}  // namespace BuiltIn
