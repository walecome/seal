#pragma once

#include <vector>

#include "Constants.hh"
#include "Quad.hh"

class FunctionDecl;

class IrFunction {
    MAKE_DEFAULT_CONSTRUCTABLE(IrFunction)
    MAKE_NONCOPYABLE(IrFunction)
    MAKE_NONMOVABLE(IrFunction)

   public:
    // @FIXME: Should not take by value
    void emplace_quad(OPCode op_code, Operand dest, Operand src_a,
                      Operand src_b);

    static Operand create_immediate(unsigned long value);
    static Operand create_label();
    static Operand create_variable();

   private:
    void dump_quads() const;

    const auto &quads() const { return m_quads; }

    std::vector<ptr_t<Quad>> m_quads {};
};