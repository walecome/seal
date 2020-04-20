#include <fmt/format.h>

#include "ir/IrFunction.hh"
#include "ir/Operand.hh"

bool Operand::is_number() const { return is_integer() || is_real(); }

bool Operand::is_integer() const {
    return is_value() && as_value().holds<IntOperand>();
}

bool Operand::is_real() const {
    return is_value() && as_value().holds<RealOperand>();
}

bool Operand::is_string() const {
    return is_value() && as_value().holds<StringOperand>();
}

bool Operand::is_value() const { return holds<ValueOperand>(); }
bool Operand::is_label() const { return holds<LabelOperand>(); }
bool Operand::is_variable() const { return holds<VariableOperand>(); }
bool Operand::is_function() const { return holds<FunctionOperand>(); }

ValueOperand Operand::as_value() const {
    ASSERT(is_value());
    return std::get<ValueOperand>(m_data);
}

VariableOperand Operand::as_variable() const {
    ASSERT(is_variable());
    return std::get<VariableOperand>(m_data);
}

FunctionOperand Operand::as_function() const {
    ASSERT(is_function());
    return std::get<FunctionOperand>(m_data);
}

LabelOperand Operand::as_label() const {
    ASSERT(is_label());
    return std::get<LabelOperand>(m_data);
}

struct ValueOperandPrinter {
    std::string operator()(StringOperand value) {
        return fmt::format("{}", value);
    }
    std::string operator()(IntOperand value) {
        return fmt::format("{}", value);
    }
    std::string operator()(RealOperand value) {
        return fmt::format("{:.5f}", value);
    }
};

struct OperandPrinter {
    const Operand* context;
    std::ostringstream oss {};

    std::string operator()(ValueOperand value) {
        return std::visit(ValueOperandPrinter {}, value.value);
    }

    std::string operator()(FunctionOperand function_id) {
        return fmt::format("function#{}", function_id);
    }

    std::string operator()(VariableOperand variable) {
        return fmt::format("{}", variable.value);
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
