#pragma once
#include <vector>

#include "Constants.hh"
#include "OPCode.hh"
#include "Operand.hh"

class Quad {
    MAKE_DEFAULT_CONSTRUCTABLE(Quad)
    MAKE_NONCOPYABLE(Quad)
    MAKE_DEFAULT_MOVABLE(Quad)

   public:
    Quad(OPCode op_code, Operand dest, Operand src_a, Operand src_b)
        : m_op_code { op_code },
          m_dest { dest },
          m_src_a { src_a },
          m_src_b { src_b } {}

    std::string to_string() const;

    const std::vector<unsigned>& label_ids() const { return m_label_ids; }
    bool has_label() const { return m_has_label; }
    void add_label(unsigned label_id);
    OPCode opcode() const { return m_op_code; };
    Operand dest() const { return m_dest; }
    Operand src_a() const { return m_src_a; }
    Operand src_b() const { return m_src_b; }

   private:
    std::vector<unsigned> m_label_ids {};
    bool m_has_label { false };

    OPCode m_op_code;
    Operand m_dest;
    Operand m_src_a;
    Operand m_src_b;
};