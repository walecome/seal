#include "interpreter/StackFrame.hh"

ValueOperand StackFrame::resolve_operand(Operand operand) const {
    if (operand.is_value()) {
        return operand.as_value();
    }

    ASSERT(operand.is_variable());
    return get_variable(operand.as_variable());
}

void ArgumentWrapper::add_named_argument(VariableOperand var,
                                         ValueOperand value) {
    m_identifiers.push_back(var);
    m_values.push_back(value);
}

void ArgumentWrapper::add_argument(ValueOperand value) {
    // Add garbage to identifier to keep size correct
    m_identifiers.push_back(VariableOperand { "" });
    m_values.push_back(value);
}

const std::vector<ValueOperand>& ArgumentWrapper::value_vector() const {
    return m_values;
}
