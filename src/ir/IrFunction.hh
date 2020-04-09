#pragma once

#include <map>
#include <unordered_map>
#include <vector>

#include "Constants.hh"
#include "Quad.hh"

class FunctionDecl;

class IrFunction {
    MAKE_NONCOPYABLE(IrFunction)
    MAKE_NONMOVABLE(IrFunction)

   public:
    IrFunction(const FunctionDecl *decl) : m_decl { decl } {}

    // Construct a quad with the given opcode and operands, placing it in
    // the m_quads vector. Also binds any queued lables to the quad and
    // clears the label queue.
    // @FIXME: Should not take by value
    void add_quad(OPCode, Operand, Operand, Operand);

    Operand create_immediate_int(unsigned long value) const;
    Operand create_immediate_string(std::string_view value) const;
    Operand create_immediate_real(double value) const;

    Operand create_label() const;
    Operand create_variable(const std::string_view) const;
    Operand create_tmp_variable();
    Operand create_function_from_id(unsigned) const;

    Operand create_and_queue_label();

    // Add a label to waiting labels
    void queue_label(const Operand &);

    // Binds the queued labels to the given quad. Also clears the label queue.
    void bind_queued_labels(size_t);

    void dump_quads() const;

    // Returns the name bound to variable_id if present, otherwise
    // tmp#<variable_id>.
    std::string resolve_variable_name(unsigned) const;

    auto declaration() const { return m_decl; }

    const auto &quads() const { return m_quads; }

    std::vector<const Quad *> quads_as_pointers() const;

    unsigned id() const;

    // Return the index of the quad bound to label
    size_t quad_idx(const LabelOperand) const;

   private:
    Operand create_variable_from_id(unsigned) const;

    // Bind the given label id to the given quad
    void bind_label(LabelOperand, size_t);

    const FunctionDecl *m_decl;

    std::vector<ptr_t<Quad>> m_quads {};
    std::map<LabelOperand, size_t> m_labels {};
    std::vector<LabelOperand> m_waiting_labels {};
    std::vector<std::string> m_tmp_variables {};
};