
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

ValueOperand add_element(const std::vector<ValueOperand>& args) {
    ASSERT(args.size() == 2);
    ASSERT(args[0].is_vector());

    VectorOperand vec = args[0].as_vector();
    ValueOperand element = args[1];

    vec.value->push_back(element);

    return IrFunction::create_immediate_int(vec.value->size());
}

}
