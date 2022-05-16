#include <fmt/format.h>
#include <variant>

#include "ir/IrFunction.hh"
#include "ir/Operand.hh"

bool ValueOperand::is_vector() const {
    return holds<VectorOperand>();
}
bool ValueOperand::is_integer() const {
    return holds<IntOperand>();
}
bool ValueOperand::is_number() const {
    return is_integer() || is_real();
}
bool ValueOperand::is_real() const {
    return holds<RealOperand>();
}
bool ValueOperand::is_string() const {
    return holds<StringOperand>();
}

VectorOperand ValueOperand::as_vector() const {
    ASSERT(is_vector());
    return get_as<VectorOperand>();
}

IntOperand ValueOperand::as_int() const {
    ASSERT(is_integer());
    return get_as<IntOperand>();
}

RealOperand ValueOperand::as_real() const {
    ASSERT(is_real());
    return get_as<RealOperand>();
}

StringOperand ValueOperand::as_string() const {
    ASSERT(is_string());
    return get_as<StringOperand>();
}

bool Operand::is_function() const {
    return holds<FunctionOperand>();
}

bool Operand::is_register() const {
    return holds<Register>();
}

bool Operand::is_label() const {
    return holds<LabelOperand>();
}

bool Operand::is_value() const {
    return holds<ValueOperand>();
}

bool Operand::is_variable() const {
    return holds<VariableOperand>();
}

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

Register Operand::as_register() const {
    ASSERT(is_register());
    return std::get<Register>(m_data);
}

LabelOperand Operand::as_label() const {
    ASSERT(is_label());
    return std::get<LabelOperand>(m_data);
}

struct ValueOperandDebugPrinter {
    std::string operator()(StringOperand value) {
        return fmt::format("s{}", value.value.id);
    }
    std::string operator()(IntOperand value) {
        return fmt::format("{}", value);
    }
    std::string operator()(RealOperand value) {
        return fmt::format("{:.5f}", value);
    }

    std::string operator()(VectorOperand vec) {
        std::ostringstream oss {};
        oss << "[";
        for (ValueOperand& value : *vec.value.get()) {
            oss << std::visit(ValueOperandDebugPrinter {}, value.value) << ", ";
        }
        oss << "]";

        return oss.str();
    }

    template <class T>
    std::string operator()(T) {
        ASSERT_NOT_REACHED();
    }
};

struct OperandDebugPrinter {
    const Operand* context;
    std::ostringstream oss {};

    std::string operator()(ValueOperand value) {
        return std::visit(ValueOperandDebugPrinter {}, value.value);
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

    std::string operator()(Register reg) {
        return reg.to_string();
    }
};

struct ValueOperandValuePrinter {
    std::string operator()(StringOperand value) {
        return fmt::format("{}", value.resolve());
    }
    std::string operator()(IntOperand value) {
        return fmt::format("{}", value);
    }
    std::string operator()(RealOperand value) {
        return fmt::format("{:.5f}", value);
    }

    std::string operator()(VectorOperand vec) {
        std::ostringstream oss {};
        oss << "[";
        for (ValueOperand& value : *vec.value.get()) {
            oss << std::visit(ValueOperandValuePrinter {}, value.value) << ", ";
        }
        oss << "]";

        return oss.str();
    }

    template <class T>
    std::string operator()(T) {
        ASSERT_NOT_REACHED();
    }
};

struct OperandValuePrinter {
    const Operand* context;
    std::ostringstream oss {};

    std::string operator()(ValueOperand value) {
        return std::visit(ValueOperandValuePrinter {}, value.value);
    }

    template <class T>
    std::string operator()(T) {
        ASSERT_NOT_REACHED();
    }
};

std::string Operand::to_debug_string() const {
    return std::visit(OperandDebugPrinter { this }, m_data);
}

std::string ValueOperand::to_debug_string() const {
    return std::visit(ValueOperandDebugPrinter {}, this->value);
}

std::string Operand::to_value_string() const {
    return std::visit(OperandValuePrinter { this }, m_data);
}

std::string ValueOperand::to_value_string() const {
    return std::visit(ValueOperandValuePrinter {}, this->value);
}
