
#include "BuiltIn.hh"
#include "ir/IrFunction.hh"
#include "ast/ArgumentList.hh"

namespace BuiltIn {


ValueOperand create_array(const std::vector<ValueOperand>& args) {
    ValueOperand arr = IrFunction::create_vector_operand();

    ASSERT(!args.empty() && args[0].is_integer());
    arr.as_vector().value->resize(args[0].as_int().value);
    return arr;
}

}
