#include <iomanip>

#include "ir/IrFunction.hh"
#include "ir/Operand.hh"

bool Operand::is_number() const { return is_integer() || is_real(); }

bool Operand::is_integer() const {
    return m_kind == OperandKind::IMMEDIATE_INT;
}

bool Operand::is_real() const { return m_kind == OperandKind::IMMEDIATE_REAL; }

bool Operand::is_string() const {
    return m_kind == OperandKind::IMMEDIATE_STRING;
}

bool Operand::is_label() const { return m_kind == OperandKind::LABEL; }
bool Operand::is_variable() const { return m_kind == OperandKind::VARIABLE; }

std::string_view Operand::variable_name() const {
    ASSERT(m_kind == OperandKind::VARIABLE);
    return env()->resolve_variable_name(std::get<VariableOperand>(m_data));
}

std::string Operand::to_string() const {
    std::ostringstream oss {};

    // TODO: All of this can be changed to a std::visit instead

    switch (m_kind) {
        case OperandKind::FUNCTION:
            oss << "function#" << std::get<FunctionOperand>(m_data);
            return oss.str();

        case OperandKind::IMMEDIATE_INT:
            oss << std::get<IntOperand>(m_data);
            return oss.str();

        case OperandKind::IMMEDIATE_REAL:
            oss << std::get<RealOperand>(m_data);
            return oss.str();

        case OperandKind::IMMEDIATE_STRING:
            oss << (std::string_view)std::get<StringOperand>(m_data);
            return oss.str();

        case OperandKind::LABEL:
            oss << std::get<LabelOperand>(m_data);
            return oss.str();

        case OperandKind::VARIABLE: {
            auto var_id = std::get<VariableOperand>(m_data);

            oss << env()->resolve_variable_name(var_id) << " (" << var_id
                << ")";
            return oss.str();
        }

        default:
            ASSERT_NOT_REACHED();
    }
}