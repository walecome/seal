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

union OperandValue {
    unsigned long integer;
    double real;
    std::string_view str {};
};

union OperandData {
    OperandValue value {};
    int label_id;
    int variable_id;
    int function_id;
};

class IrFunction;

class Operand {
   public:
    Operand() : m_kind { OperandKind::UNUSED }, m_data {} {}

    Operand(OperandKind kind, OperandData data)
        : m_kind { kind }, m_data { data } {}

    OperandKind& kind() { return m_kind; }
    OperandData& data() { return m_data; }

    const OperandData& data() const { return m_data; }
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
    OperandData m_data;

    const IrFunction* m_env { nullptr };
};