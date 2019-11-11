#include "Operand.hh"

Operand Operand::create_immediate(const OperandType&& type,
                                  unsigned long value) {
    Operand operand {};
    operand.m_type = type;
    operand.m_kind = OperandKind::IMMEDIATE;
    operand.m_data.immediate_value = value;

    return operand;
}

Operand Operand::create_label(const std::string&& name) {}

Operand Operand::create_variable(const OperandType&& type,
                                 const std::string&& name) {}

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