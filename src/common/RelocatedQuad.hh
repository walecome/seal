#pragma once

#include <optional>
#include <variant>

#include "Constants.hh"
#include "common/BuiltinFunctionAddress.hh"
#include "common/ConstantPool.hh"
#include "common/Register.hh"
#include "interpreter/InstructionAddress.hh"
#include "ir/OPCode.hh"

#include "common/Operand.hh"

class RelocatedQuad {
   public:
    struct Stringified {};

    RelocatedQuad(OPCode op_code, Operand dest, Operand src_a, Operand src_b);
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
