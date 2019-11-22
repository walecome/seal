#pragma once

#include "Constants.hh"
#include "OperandType.hh"

enum class OperandKind {
    IMMEDIATE_INT,
    IMMEDIATE_REAL,
    IMMEDIATE_STRING,
    VARIABLE,
    LABEL,
    FUNCTION,
};

union OperandData {
    unsigned long immediate_int;
    double immediate_real;
    std::string_view immediate_string {};
    int label_id;
    int variable_id;
    int function_id;
};

class IrFunction;

class Operand {
   public:
    Operand() : m_kind {}, m_data {}, m_used { false } {}

    Operand(OperandKind kind, OperandData data)
        : m_kind { kind }, m_data { data }, m_used { true } {}

    OperandKind& kind() { return m_kind; }
    OperandData& data() { return m_data; }

    const OperandData& data() const { return m_data; }
    const OperandKind& kind() const { return m_kind; }

    bool is_immediate_int() const;
    bool is_immediate_real() const;
    bool is_immediate_string() const;
    bool is_label() const;
    bool is_variable() const;

    bool is_used() const { return m_used; }

    auto env() const { return m_env; }
    void set_env(const IrFunction* env) { m_env = env; }

    std::string to_string() const;

   private:
    OperandKind m_kind;
    OperandData m_data;

    const IrFunction* m_env { nullptr };

    bool m_used;
};