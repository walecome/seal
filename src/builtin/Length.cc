#include "BuiltIn.hh"
#include "ir/IrFunction.hh"
#include "ast/ArgumentList.hh"

namespace BuiltIn {

// fn get_length(iterable: string | []) -> int
ValueOperand get_length(const std::vector<ValueOperand>& args) {
    ASSERT(args.size() == 1);
    ASSERT(args[0].is_vector() || args[0].is_string());

    const ValueOperand& operand = args[0];

    if (operand.is_string()) {
        return IrFunction::create_immediate_int(operand.as_string().resolve()->size());
    }

    if (operand.is_vector()) {
        return IrFunction::create_immediate_int(operand.as_vector().value->size());
    }

    ASSERT_NOT_REACHED();
}

}
