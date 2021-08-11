#pragma once
#include <tuple>
#include <vector>

#include "Constants.hh"
#include "OPCode.hh"
#include "Operand.hh"
#include "Register.hh"
#include "QuadSource.hh"
#include "QuadDest.hh"

class Quad {
   public:
    Quad(OPCode op_code, QuadDest dest, QuadSource src_a, QuadSource src_b)
        : m_op_code { op_code },
          m_dest { dest },
          m_src_a { src_a },
          m_src_b { src_b } {}

    std::string to_string() const;
    std::tuple<std::string, std::string, std::string, std::string, std::string>
    to_string_segments() const;

    const std::vector<unsigned>& label_ids() const { return m_label_ids; }
    bool has_label() const { return m_has_label; }
    void add_label(unsigned label_id);
    OPCode opcode() const { return m_op_code; };
    QuadDest dest() const { return m_dest; }
    QuadSource src_a() const { return m_src_a; }
    QuadSource src_b() const { return m_src_b; }

   private:
    std::vector<unsigned> m_label_ids {};
    bool m_has_label { false };

    OPCode m_op_code;
    QuadDest m_dest;
    QuadSource m_src_a;
    QuadSource m_src_b;
};