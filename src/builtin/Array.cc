
#include "BuiltIn.hh"
#include "ir/IrFunction.hh"
#include "ast/ArgumentList.hh"
#include "interpreter/Context.hh"

namespace BuiltIn {

PoolEntry create_array(const std::vector<PoolEntry>& args, Context& context) {
    ASSERT(args.size() == 1);
    Value& size = context.get_value(args[0]);
    ASSERT(size.is_integer());

    std::vector<PoolEntry> vec;

    vec.resize(size.as_integer().value());
    return context.dynamic_pool().create_vector(vec);
}

PoolEntry add_element(const std::vector<PoolEntry>& args, Context& context) {
    ASSERT(args.size() == 2);
    Value& vec = context.get_value(args[0]);
    ASSERT(vec.is_vector());

    vec.as_vector().add(args[1]);

    return context.dynamic_pool().create_integer(vec.as_vector().length());
}

}
