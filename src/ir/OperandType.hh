#pragma once

#include "Constants.hh"

enum class ValueKind { SIGNED, UNSIGNED, PTR };

class OperandType {
    MAKE_DEFAULT_CONSTRUCTABLE(OperandType)

   public:
    OperandType(ValueKind kind, unsigned size)
        : m_kind { kind }, m_size { size } {}

    std::string to_string() const;

   private:
    ValueKind m_kind;
    unsigned m_size;
};