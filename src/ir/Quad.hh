#pragma once
#include <tuple>
#include <vector>

#include "Constants.hh"
#include "OPCode.hh"
#include "ir/IrOperand.hh"
#include "common/Register.hh"

class Quad {
   public:
    Quad(OPCode op_code, IrOperand dest, IrOperand src_a, IrOperand src_b,
         bool needs_relocation)
        : m_op_code { op_code },
          m_dest { dest },
          m_src_a { src_a },
          m_src_b { src_b },
          m_needs_relocation { needs_relocation } {
    }

    std::string to_string() const;
    std::tuple<std::string, std::string, std::string, std::string, std::string>
    to_string_segments() const;

    const std::vector<unsigned>& label_ids() const {
        return m_label_ids;
    }
    bool has_label() const {
        return m_has_label;
    }
    void add_label(unsigned label_id);
    OPCode opcode() const {
        return m_op_code;
    };
    IrOperand dest() const {
        return m_dest;
    }
    IrOperand src_a() const {
        return m_src_a;
    }
    IrOperand src_b() const {
        return m_src_b;
    }
    bool needs_relocation() const {
        return m_needs_relocation;
    }

   private:
    std::vector<unsigned> m_label_ids {};
    bool m_has_label { false };

    OPCode m_op_code;
    IrOperand m_dest;
    IrOperand m_src_a;
    IrOperand m_src_b;
    bool m_needs_relocation;
};
