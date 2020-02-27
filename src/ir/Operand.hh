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
#define ADD_LANGUAGE_TYPE(name, type)           \
    struct name {                               \
        name() = default;                       \
        type value {};                          \
        operator type() const { return value; } \
    };

// For immediate operands
ADD_LANGUAGE_TYPE(IntOperand, unsigned long)
ADD_LANGUAGE_TYPE(RealOperand, double)
ADD_LANGUAGE_TYPE(StringOperand, std::string_view)

// For named operands (label, variable, function)
ADD_LANGUAGE_TYPE(LabelOperand, unsigned)
ADD_LANGUAGE_TYPE(VariableOperand, unsigned)
ADD_LANGUAGE_TYPE(FunctionOperand, unsigned)

using operand_type_t =
    std::variant<IntOperand, RealOperand, StringOperand, LabelOperand,
                 VariableOperand, FunctionOperand>;

class IrFunction;

class Operand {
   public:
    Operand() : m_kind { OperandKind::UNUSED }, m_data {} {}

    Operand(OperandKind kind, operand_type_t data)
        : m_kind { kind }, m_data { data } {}

    OperandKind& kind() { return m_kind; }
    operand_type_t& data() { return m_data; }

    const operand_type_t& data() const { return m_data; }
    const OperandKind& kind() const { return m_kind; }

    bool is_number() const;
    bool is_integer() const;
    bool is_real() const;
    bool is_string() const;
    bool is_label() const;
    bool is_variable() const;

    bool is_used() const { return m_kind != OperandKind::UNUSED; }

    std::string_view variable_name() const;

    auto env() const { return m_env; }
    void set_env(const IrFunction* env) { m_env = env; }

    std::string to_string() const;

   private:
    OperandKind m_kind;
    operand_type_t m_data;

    const IrFunction* m_env { nullptr };
};