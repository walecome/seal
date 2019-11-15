#include "ir/Operand.hh"
#include "ir/IrFunction.hh"

bool Operand::is_immediate() const { return m_kind == OperandKind::IMMEDIATE; }
bool Operand::is_label() const { return m_kind == OperandKind::LABEL; }
bool Operand::is_variable() const { return m_kind == OperandKind::VARIABLE; }

std::string Operand::to_string() const {
    std::ostringstream oss {};

    switch (m_kind) {
        case OperandKind::FUNCTION:
            oss << "function#" << m_data.function_id;
            return oss.str();

        case OperandKind::IMMEDIATE:
            oss << "#" << m_data.immediate_value;
            return oss.str();

        case OperandKind::LABEL:
            oss << "L" << m_data.label_id;
            return oss.str();

        case OperandKind::VARIABLE:
            oss << env()->resolve_variable_name(m_data.variable_id) << " ("
                << m_data.variable_id << ")";
            return oss.str();

        default:
            ASSERT_NOT_REACHED();
    }
}