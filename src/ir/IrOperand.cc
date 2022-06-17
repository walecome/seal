#include <fmt/format.h>
#include <variant>

#include "ir/IrFunction.hh"
#include "ir/IrOperand.hh"

bool IrOperand::is_function() const {
    return holds<FunctionOperand>();
}

bool IrOperand::is_register() const {
    return holds<Register>();
}

bool IrOperand::is_label() const {
    return holds<LabelOperand>();
}

bool IrOperand::is_constant_entry() const {
    return holds<ConstantPool::Entry>();
}

ConstantPool::Entry IrOperand::as_constant_entry() const {
    ASSERT(is_constant_entry());
    return std::get<ConstantPool::Entry>(m_data.value());
}

FunctionOperand IrOperand::as_function() const {
    ASSERT(is_function());
    return std::get<FunctionOperand>(m_data.value());
}

Register IrOperand::as_register() const {
    ASSERT(is_register());
    return std::get<Register>(m_data.value());
}

LabelOperand IrOperand::as_label() const {
    ASSERT(is_label());
    return std::get<LabelOperand>(m_data.value());
}

struct OperandDebugPrinter {
    const IrOperand* context;
    std::ostringstream oss {};

    std::string operator()(ConstantPool::Entry entry) {
        return entry.to_string();
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

std::string IrOperand::to_debug_string() const {
    if (!m_data.has_value()) {
      return "NO VALUE";
    }
    return std::visit(OperandDebugPrinter { this }, m_data.value());
}
