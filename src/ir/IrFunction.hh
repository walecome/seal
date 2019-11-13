#pragma once

#include <map>
#include <vector>

#include "Constants.hh"
#include "Quad.hh"

class FunctionDecl;

class IrFunction {
    MAKE_DEFAULT_CONSTRUCTABLE(IrFunction)
    MAKE_NONCOPYABLE(IrFunction)
    MAKE_NONMOVABLE(IrFunction)

   public:
    // Construct a quad with the given opcode and operands, placing it in the
    // m_quads vector. Also binds any queued lables to the quad and clears the
    // label queue.
    // @FIXME: Should not take by value
    void add_quad(OPCode op_code, Operand dest, Operand src_a, Operand src_b);

    static Operand create_immediate(unsigned long value);
    static Operand create_label();
    static Operand create_variable();

    // Add a label to waiting labels
    void queue_label(const Operand &label);

    // Binds the queued labels to the given quad. Also clears the label queue.
    void bind_queued_labels(const Quad *quad);

    // Bind the given label to the given quad
    void bind_label(const Operand &label, const Quad *quad);

   private:
    void dump_quads() const;
    const auto &quads() const { return m_quads; }

    // Bind the given label id to the given quad
    void bind_label(unsigned label_id, const Quad *quad);

    std::vector<ptr_t<Quad>> m_quads {};
    std::map<unsigned, const Quad *> m_labels {};
    std::vector<unsigned> m_waiting_labels {};
};