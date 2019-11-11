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
    MAKE_DEFAULT_CONSTRUCTABLE(Operand)

   public:
    static Operand create_immediate(const OperandType&& type,
                                    unsigned long value);
    static Operand create_label(const std::string&& name);
    static Operand create_variable(const OperandType&& type,
                                   const std::string&& name);

    bool is_immediate() const;
    bool is_label() const;
    bool is_variable() const;

    std::string to_string() const;

   private:
    OperandKind m_kind;
    OperandType m_type;
    OperandData m_data;
};