#include <fmt/format.h>

#include "ir/IrFunction.hh"
#include "ir/Operand.hh"

bool ValueOperand::is_vector() const { return holds<VectorOperand>(); }
bool ValueOperand::is_integer() const { return holds<IntOperand>(); }
bool ValueOperand::is_number() const { return is_integer() || is_real(); }
bool ValueOperand::is_real() const { return holds<RealOperand>(); }
bool ValueOperand::is_string() const { return holds<StringOperand>(); }

VectorOperand::value_type_t ValueOperand::as_vector() const {
    ASSERT(is_vector());
    return get_as<VectorOperand>();
}

unsigned long ValueOperand::as_int() const {
    ASSERT(is_integer());
    return get_as<IntOperand>();
}

double ValueOperand::as_real() const {
    ASSERT(is_real());
    return get_as<RealOperand>();
}

std::string_view ValueOperand::as_string() const {
    ASSERT(is_string());
    return get_as<StringOperand>();
}

bool Operand::is_function() const { return holds<FunctionOperand>(); }
bool Operand::is_label() const { return holds<LabelOperand>(); }
bool Operand::is_value() const { return holds<ValueOperand>(); }
bool Operand::is_variable() const { return holds<VariableOperand>(); }

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

    std::string operator()(VectorOperand vec) {
        std::ostringstream oss {};
        oss << "[";
        for (ValueOperand& value : *vec.value.get()) {
            oss << std::visit(ValueOperandPrinter {}, value.value) << ", ";
        }
        oss << "]";

        return oss.str();
    }

    template <class T>
    std::string operator()(T) {
        ASSERT_NOT_REACHED();
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
