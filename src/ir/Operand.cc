#include <fmt/format.h>
#include <variant>

#include "ir/IrFunction.hh"
#include "ir/Operand.hh"

bool Operand::is_function() const {
    return holds<FunctionOperand>();
}

bool Operand::is_register() const {
    return holds<Register>();
}

bool Operand::is_label() const {
    return holds<LabelOperand>();
}

bool Operand::is_value_entry() const {
    return holds<PoolEntry>();
}

PoolEntry Operand::as_value_entry() const {
    ASSERT(is_value_entry());
    return std::get<PoolEntry>(m_data);
}

FunctionOperand Operand::as_function() const {
    ASSERT(is_function());
    return std::get<FunctionOperand>(m_data);
}

Register Operand::as_register() const {
    ASSERT(is_register());
    return std::get<Register>(m_data);
}

LabelOperand Operand::as_label() const {
    ASSERT(is_label());
    return std::get<LabelOperand>(m_data);
}

struct OperandDebugPrinter {
    const Operand* context;
    std::ostringstream oss {};

    std::string operator()(PoolEntry entry) {
        return fmt::format("C#{}", entry.key);
    }

    std::string operator()(FunctionOperand function_id) {
        return fmt::format("function#{}", function_id);
    }

    std::string operator()(LabelOperand label_id) {
        return fmt::format("L#{}", label_id);
    }

    std::string operator()(Register reg) {
        return reg.to_string();
    }
};

std::string Operand::to_debug_string() const {
    return std::visit(OperandDebugPrinter { this }, m_data);
}
