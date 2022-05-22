#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"
#include "interpreter/Context.hh"
#include "ir/IrFunction.hh"

namespace BuiltIn {

// fn get_length(iterable: string | []) -> int
PoolEntry get_length(const std::vector<PoolEntry>& args, Context& context) {
    ASSERT(args.size() == 1);

    const Value& target = context.get_value(args[0]);

    if (target.is_string()) {
        return context.dynamic_pool().create_integer(
            target.as_string().length());
    }

    if (target.is_vector()) {
        return context.dynamic_pool().create_integer(
            target.as_vector().length());
    }

    ASSERT_NOT_REACHED();
}

}  // namespace BuiltIn
