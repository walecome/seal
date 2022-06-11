#pragma once

#include <optional>
#include <variant>

#include "Constants.hh"
#include "common/ConstantPool.hh"
#include "common/Register.hh"
#include "interpreter/InstructionAddress.hh"
#include "ir/OPCode.hh"

class RelocatedQuad {
   public:
    class Operand {
       public:
        explicit Operand(InstructionAddress addr);
        explicit Operand(Register reg);
        explicit Operand(ConstantPool::Entry constant);

        static Operand empty();

        bool is_address() const;
        bool is_register() const;
        bool is_constant() const;
        bool is_used() const;

        InstructionAddress as_address() const;
        Register as_register() const;
        ConstantPool::Entry as_constant() const;

       private:
        Operand();

        std::optional<
            std::variant<InstructionAddress, Register, ConstantPool::Entry>>
            m_data;
    };

    struct Stringified {};

    RelocatedQuad(OPCode op_code, Operand dest, Operand src_a, Operand src_b)
        : m_op_code { op_code },
          m_dest { dest },
          m_src_a { src_a },
          m_src_b { src_b } {
    }

    Stringified stringify() const;

    OPCode opcode() const;
    Operand dest() const;
    Operand src_a() const;
    Operand src_b() const;

   private:
    OPCode m_op_code;
    Operand m_dest;
    Operand m_src_a;
    Operand m_src_b;
};
