#include <fmt/format.h>

#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"
#include "interpreter/Context.hh"
#include "ir/IrFunction.hh"

namespace BuiltIn {

PoolEntry print(const std::vector<PoolEntry> &args, Context &context) {
    std::vector<std::string> stringified_args {};

    for (auto &arg : args) {
        stringified_args.push_back(context.get_value(arg).to_string(context));
    }

    std::string s = fmt::format("{}", fmt::join(stringified_args, ""));
    fmt::print("{}", s);

    return context.dynamic_pool().create_integer(s.size());
}

PoolEntry println(const std::vector<PoolEntry> &args, Context &context) {
    unsigned long chars_printed =
        context.get_value(print(args, context)).as_integer().value();
    fmt::print("\n");
    return context.dynamic_pool().create_integer(chars_printed + 1);
}
}  // namespace BuiltIn
