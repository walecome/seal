#include "common/RelocatedQuad.hh"

RelocatedQuad::RelocatedQuad(OPCode op_code, Operand dest, Operand src_a,
                             Operand src_b)
    : m_op_code { op_code },
      m_dest { dest },
      m_src_a { src_a },
      m_src_b { src_b } {
}

RelocatedQuad::Stringified RelocatedQuad::stringify() const {
    Stringified stringified;

    stringified.opcode = opcode_to_string(opcode());
    stringified.dest = dest().to_debug_string();
    stringified.src_a = src_a().to_debug_string();
    stringified.src_b = src_b().to_debug_string();

    return stringified;

}

OPCode RelocatedQuad::opcode() const {
    return m_op_code;
}

Operand RelocatedQuad::dest() const {
    return m_dest;
}

Operand RelocatedQuad::src_a() const {
    return m_src_a;
}

Operand RelocatedQuad::src_b() const {
    return m_src_b;
}
