#include "interpreter/StackFrame.hh"

ValueOperand StackFrame::resolve_operand(Operand operand) const {
    if (operand.is_value()) {
        return operand.as_value();
    }

    ASSERT(operand.is_variable());
    return get_variable(operand.as_variable());
}
