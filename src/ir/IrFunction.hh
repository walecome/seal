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
    void add_quad(OPCode op_code, Operand dest, Operand src_a, Operand src_b);

    Operand create_immediate_int(unsigned long value) const;
    Operand create_immediate_string(std::string_view value) const;
    Operand create_immediate_real(double value) const;

    Operand create_label() const;
    Operand create_variable() const;
    Operand create_function_from_id(unsigned function_id) const;

    Operand get_variable(std::string_view identifier) const;

    Operand create_and_queue_label();

    // Add a label to waiting labels
    void queue_label(const Operand &label);

    // Binds the queued labels to the given quad. Also clears the label queue.
    void bind_queued_labels(Quad *quad);

    // Bind the given label to the given quad
    void bind_label(const Operand &label, Quad *quad);

    // Bind a source code variable name to an IR variable
    void bind_variable(const Operand &variable,
                       const std::string_view var_name);

    void dump_quads() const;

    // Returns the name bound to variable_id if present, otherwise
    // tmp#<variable_id>.
    std::string_view resolve_variable_name(unsigned variable_id) const;

    auto declaration() const { return m_decl; }

    void __dump_variables() const;

   private:
    Operand create_variable_from_id(unsigned id) const;

    const auto &quads() const { return m_quads; }

    // Bind the given label id to the given quad
    void bind_label(unsigned label_id, Quad *quad);

    const FunctionDecl *m_decl;

    std::vector<ptr_t<Quad>> m_quads {};
    std::map<unsigned, Quad *> m_labels {};
    std::vector<unsigned> m_waiting_labels {};
    std::unordered_map<unsigned, std::string_view> m_variable_ref {};
    std::unordered_map<std::string_view, unsigned> m_varname_to_id {};
};