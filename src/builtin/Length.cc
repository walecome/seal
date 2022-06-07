#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"
#include "ir/IrFunction.hh"

namespace BuiltIn {

// fn get_length(iterable: string | []) -> int
Value get_length(const std::vector<Value>& args) {
    ASSERT(args.size() == 1);

    Value target = args[0];

    if (target.is_string()) {
        return Value::create_integer(target.as_string().length());
    }

    if (target.is_vector()) {
        return Value::create_integer(target.as_vector().length());
    }

    ASSERT_NOT_REACHED();
}

}  // namespace BuiltIn
