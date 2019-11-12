#include "Operand.hh"

bool Operand::is_immediate() const { return m_kind == OperandKind::IMMEDIATE; }
bool Operand::is_label() const { return m_kind == OperandKind::LABEL; }
bool Operand::is_variable() const { return m_kind == OperandKind::VARIABLE; }

std::string Operand::to_string() const {
    std::ostringstream oss {};

    switch (m_kind) {
        case OperandKind::IMMEDIATE:

            oss << "Immediate " << m_data.immediate_value;
            return oss.str();

        case OperandKind::LABEL:
            oss << "Label " << m_data.label_id;
            return oss.str();

        case OperandKind::VARIABLE:
            oss << "Variable " << m_data.variable_id;
            return oss.str();

        default:
            ASSERT_NOT_REACHED();
    }
}