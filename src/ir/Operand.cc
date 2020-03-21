#include <iomanip>

#include <fmt/format.h>

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
bool Operand::is_function() const { return m_kind == OperandKind::FUNCTION; }

std::string_view Operand::variable_name() const {
    ASSERT(m_kind == OperandKind::VARIABLE);
    return env()->resolve_variable_name(std::get<VariableOperand>(m_data));
}

struct ValueOperandPrinter {
    std::string operator()(IntOperand value) {
        return fmt::format("{}", value);
    }

    std::string operator()(RealOperand value) {
        return fmt::format("{:f}", value);
    }

    std::string operator()(StringOperand value) {
        return fmt::format("\"{}\"", value);
    }
};

struct OperandPrinter {
    const Operand* context;

    std::string operator()(ValueOperand value) {
        return std::visit(ValueOperandPrinter {}, value.value);
    }

    std::string operator()(FunctionOperand function_id) {
        return fmt::format("function#{}", function_id);
    }

    std::string operator()(VariableOperand var_id) {
        return fmt::format(
            "{}({})",
            std::string(context->env()->resolve_variable_name(var_id)), var_id);
    }

    std::string operator()(LabelOperand label_id) {
        return fmt::format("L#{}", label_id);
    }
};

std::string Operand::to_string() const {
    return std::visit(OperandPrinter { this }, m_data);
}

void print_value_operand(value_operand_t value) {
    fmt::print("{}\n", std::visit(ValueOperandPrinter {}, value));
}

std::string value_operand_to_string(ValueOperand value) {
    return std::visit(ValueOperandPrinter {}, value.value);
}
