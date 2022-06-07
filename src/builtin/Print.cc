#include <fmt/format.h>

#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"
#include "ir/IrFunction.hh"

namespace BuiltIn {

Value print(const builtin_args_t& args) {
    std::vector<std::string> stringified_args {};

    for (auto arg : args) {
        stringified_args.push_back(arg.stringify());
    }

    std::string s = fmt::format("{}", fmt::join(stringified_args, ""));
    fmt::print("{}", s);

    return Value::create_integer(s.size());
}

Value println(const builtin_args_t& args) {
    unsigned long chars_printed = print(args).as_integer().value();
    fmt::print("\n");
    return Value::create_integer(chars_printed + 1);
}
}  // namespace BuiltIn
