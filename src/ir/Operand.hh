#pragma once

#include <variant>

#include "Constants.hh"
#include "OperandType.hh"

enum class OperandKind {
    IMMEDIATE_INT,
    IMMEDIATE_REAL,
    IMMEDIATE_STRING,
    VARIABLE,
    LABEL,
    FUNCTION,
    UNUSED,
};
// This defines a struct with the identifier name. In addition it adds a member
// named value of the given type, as well as an overload for that type.
#define ADD_OPERAND_TYPE(NAME, TYPE)                                         \
    struct NAME {                                                            \
        NAME() = default;                                                    \
        TYPE value {};                                                       \
        operator TYPE() const { return value; }                              \
        friend std::ostream& operator<<(std::ostream& os, const NAME& cls) { \
            os << cls.value;                                                 \
            return os;                                                       \
        }                                                                    \
    };

// For immediate operands
ADD_OPERAND_TYPE(IntOperand, unsigned long)
ADD_OPERAND_TYPE(RealOperand, double)
ADD_OPERAND_TYPE(StringOperand, std::string_view)

using value_operand_t = std::variant<IntOperand, RealOperand, StringOperand>;

struct ValueOperand {
    value_operand_t value {};

    unsigned long as_int() const {
        ASSERT(std::holds_alternative<IntOperand>(value));
        return std::get<IntOperand>(value).value;
    }

    double as_real() const {
        ASSERT(std::holds_alternative<RealOperand>(value));
        return std::get<RealOperand>(value).value;
    }

    std::string_view as_string() const {
        ASSERT(std::holds_alternative<StringOperand>(value));
        return std::get<StringOperand>(value).value;
    }
};

// For named operands (label, variable, function)
ADD_OPERAND_TYPE(LabelOperand, unsigned)
ADD_OPERAND_TYPE(VariableOperand, std::string_view)
ADD_OPERAND_TYPE(FunctionOperand, unsigned)

using operand_type_t =
    std::variant<ValueOperand, LabelOperand, VariableOperand, FunctionOperand>;

class IrFunction;

struct OperandPrinter;

class Operand {
   public:
    Operand() : m_kind { OperandKind::UNUSED }, m_data {} {}

    Operand(OperandKind kind, operand_type_t data)
        : m_kind { kind }, m_data { data } {}

    bool is_number() const;
    bool is_integer() const;
    bool is_real() const;
    bool is_string() const;
    bool is_value() const;

    bool is_label() const;
    bool is_variable() const;
    bool is_function() const;

    bool is_used() const { return m_kind != OperandKind::UNUSED; }

    auto env() const { return m_env; }
    void set_env(const IrFunction* env) { m_env = env; }

    std::string to_string() const;

    ValueOperand as_value() const;
    VariableOperand as_variable() const;
    LabelOperand as_label() const;
    FunctionOperand as_function() const;

   private:
    OperandKind m_kind;
    operand_type_t m_data;

    const IrFunction* m_env { nullptr };
};

void print_value_operand(value_operand_t);
std::string value_operand_to_string(ValueOperand);
