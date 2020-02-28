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

struct OperandPrinter {
    const Operand* context;
    std::ostringstream oss {};

    template <typename T>
    std::string operator()(T value) {
        oss << value;
        return oss.str();
    }

    std::string operator()(FunctionOperand function_id) {
        oss << "function#" << function_id;
        return oss.str();
    }

    std::string operator()(VariableOperand var_id) {
        oss << context->env()->resolve_variable_name(var_id) << " (" << var_id
            << ")";
        return oss.str();
    }
};

std::string Operand::to_string() const {
    return std::visit(OperandPrinter { this }, m_data);
}