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
    Operand() : m_kind {}, m_data {}, used { false } {}

    OperandKind& kind() { return m_kind; }
    OperandData& data() { return m_data; }

    bool is_immediate() const;
    bool is_label() const;
    bool is_variable() const;

    std::string to_string() const;

   private:
    OperandKind m_kind;
    OperandData m_data;

    bool used;
};