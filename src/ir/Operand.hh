#pragma once

#include "Constants.hh"
#include "OperandType.hh"

enum class OperandKind {
    IMMEDIATE,
    VARIABLE,
    LABEL,
};

union OperandData {
    unsigned long immediate_value;
    int label_id;
    int variable_id;
};

class Operand {
   public:
    Operand() : m_kind {}, m_data {}, m_used { false } {}

    OperandKind& kind() { return m_kind; }
    OperandData& data() { return m_data; }

    const OperandData& data() const { return m_data; }
    const OperandKind& kind() const { return m_kind; }

    bool is_immediate() const;
    bool is_label() const;
    bool is_variable() const;

    bool is_used() const { return m_used; }

    std::string to_string() const;

   private:
    OperandKind m_kind;
    OperandData m_data;

    bool m_used;
};