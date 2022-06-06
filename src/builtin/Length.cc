#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"
#include "interpreter/Context.hh"
#include "ir/IrFunction.hh"

namespace BuiltIn {

// fn get_length(iterable: string | []) -> int
Value get_length(const std::vector<Value>& args) {
    ASSERT(args.size() == 1);

    Value target = args[0];

    if (target.is<String>()) {
        return Value::create_integer(target.as<String>().length());
    }

    if (target.is<Vector>()) {
        return Value::create_integer(target.as<Vector>().length());
    }

    ASSERT_NOT_REACHED();
}

}  // namespace BuiltIn
