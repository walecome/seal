#include "interpreter/StackFrame.hh"

void StackFrame::set_variable(VariableOperand var, ValueOperand value,
                              bool force_current_frame) {
    StackFrame* target_frame = nullptr;

    if (!force_current_frame) {
        target_frame = get_variable_frame(var);
    }

    if (target_frame) {
        target_frame->assign_variable(var, value);
    } else {
        // No frame had the variable previously set
        m_variables[var] = value;
    }
}

void StackFrame::assign_variable(VariableOperand var, ValueOperand value) {
    ASSERT(m_variables.count(var) == 1);

    m_variables[var] = value;
}

ValueOperand StackFrame::get_variable(VariableOperand var) const {
    auto it = m_variables.find(var);

    if (it == std::end(m_variables)) {
        return m_parent->get_variable(var);
    }

    return it->second;
}

ValueOperand StackFrame::resolve_operand(Operand operand) const {
    if (operand.is_value()) {
        return operand.as_value();
    }

    if (operand.is_variable()) {
        return get_variable(operand.as_variable());
    }

    ASSERT_NOT_REACHED_MSG("Unexpected operand resolve");
}

StackFrame* StackFrame::get_variable_frame(VariableOperand var) {
    auto it = m_variables.find(var);

    if (it == std::end(m_variables)) {
        if (!m_parent) {
            return nullptr;
        }

        return m_parent->get_variable_frame(var);
    }

    return this;
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
