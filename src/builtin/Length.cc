#include "BuiltIn.hh"
#include "ast/ArgumentList.hh"
#include "ir/IrFunction.hh"

namespace BuiltIn {

// fn get_length(iterable: string | []) -> int
Value get_length(const std::vector<Value>& args, const Context& context) {
    ASSERT(args.size() == 1);
    ASSERT(args[0].is_vector() || args[0].is_string());

    const Value& value = args[0];

    if (value.is_string()) {
      std::string_view underlaying_string = context.resolve(value.as_string());
      return context.create_value_from(Integer(underlaying_string.size()));
    }

    if (value.is_vector()) {
        size_t size = value.as_vector().values().size();
        return context.create_value_from(Integer(static_cast<int>(size)));
    }

    ASSERT_NOT_REACHED();
}

}  // namespace BuiltIn
