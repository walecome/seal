
#include "BuiltIn.hh"
#include "ir/IrFunction.hh"
#include "ast/ArgumentList.hh"

namespace BuiltIn {


ValueOperand create_array(const std::vector<ValueOperand>& args) {
    ASSERT(args.size() == 1 && args[0].is_integer());

    ValueOperand arr = IrFunction::create_vector_operand();
    arr.as_vector().value->resize(args[0].as_int().value);
    return arr;
}

}
