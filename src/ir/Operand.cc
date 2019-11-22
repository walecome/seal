#include <iomanip>

#include "ir/IrFunction.hh"
#include "ir/Operand.hh"

bool Operand::is_immediate_int() const {
    return m_kind == OperandKind::IMMEDIATE_INT;
}

bool Operand::is_immediate_real() const {
    return m_kind == OperandKind::IMMEDIATE_REAL;
}

bool Operand::is_immediate_string() const {
    return m_kind == OperandKind::IMMEDIATE_STRING;
}

bool Operand::is_label() const { return m_kind == OperandKind::LABEL; }
bool Operand::is_variable() const { return m_kind == OperandKind::VARIABLE; }

std::string Operand::to_string() const {
    std::ostringstream oss {};

    switch (m_kind) {
        case OperandKind::FUNCTION:
            oss << "function#" << m_data.function_id;
            return oss.str();

        case OperandKind::IMMEDIATE_INT:
            oss << m_data.immediate_int;
            return oss.str();

        case OperandKind::IMMEDIATE_REAL:
            oss << m_data.immediate_real;
            return oss.str();

        case OperandKind::IMMEDIATE_STRING:
            oss << std::quoted(m_data.immediate_string);
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