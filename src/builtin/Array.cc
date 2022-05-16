
#include "BuiltIn.hh"
#include "ir/IrFunction.hh"
#include "ast/ArgumentList.hh"

namespace BuiltIn {


Value create_array(const std::vector<Value>& args, const Context& context) {
    ASSERT(args.size() == 1 && args[0].is_integer());

    Vector vec = Vector();

    vec.mutable_values().resize(args[0].as_integer().value());
    return context.create_value_from(vec);
}

Value add_element(const std::vector<Value>& args, const Context& context) {
    ASSERT(args.size() == 2);
    ASSERT(args[0].is_vector());

    Vector vec = args[0].as_vector();
    // FIXME: This is not going to work, since the Vector we is a copy of the original vector.
    vec.mutable_values().push_back(args.at(1).copy());

    return context.create_value_from(Integer(vec.values().size()));
}

}
