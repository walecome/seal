#pragma once

#include "Constants.hh"

class IrVariable {
    MAKE_DEFAULT_MOVABLE(IrVariable)
    MAKE_NONCOPYABLE(IrVariable)

    static IrVariable new_variable(unsigned byte_size) {
        static unsigned variable_count = 0;
        return { variable_count++, byte_size };
    }

    IrVariable(unsigned id, unsigned byte_size)
        : m_id { id }, m_byte_size { byte_size } {}

    unsigned id() const { return m_id; }
    unsigned byte_size() const { return m_byte_size; }

   private:
    unsigned m_id;
    unsigned m_byte_size;
};