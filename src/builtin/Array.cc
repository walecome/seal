
#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"
#include "ir/IrFunction.hh"

namespace BuiltIn {

Value create_array(const std::vector<Value>& args) {
    ASSERT(args.size() == 1);

    Value size = args[0];
    ASSERT(size.is_integer());

    Value vec = Value::create_vector();

    for (int i = 0; i < size.as_integer().value(); ++i) {
        vec.as_vector().add(Value{});
    }
    
    return vec;
}

Value add_element(const std::vector<Value>& args) {
    ASSERT_NOT_REACHED_MSG("BuiltIn::add_element not implemented");
    // ASSERT(args.size() == 2);
    // Value& vec = context.get_value(args[0]);
    // ASSERT(vec.is_vector());

    // vec.as_vector().add(args[1]);

    // return context.dynamic_pool().create_integer(vec.as_vector().length());
}

}  // namespace BuiltIn
