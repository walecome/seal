#pragma once

#include <map>
#include <set>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Constants.hh"
#include "Quad.hh"
#include "Register.hh"
#include "QuadSource.hh"
#include "QuadDest.hh"

class FunctionDecl;

template <class ValueType, class T = decltype(ValueType().value)>
ValueOperand create_value_operand(T value) {
    return ValueOperand { ValueType { value } };
}

class IrFunction {
    MAKE_NONCOPYABLE(IrFunction)
    MAKE_NONMOVABLE(IrFunction)

   public:
    IrFunction(const FunctionDecl *decl) : m_decl { decl } {}

    // Construct a quad with the given opcode and operands, placing it in
    // the m_quads vector. Also binds any queued lables to the quad and
    // clears the label queue.
    // @FIXME: Should not take by value
    void add_quad(OPCode, QuadDest, QuadSource, QuadSource);

    ValueOperand create_immediate_int(unsigned long value) const;
    ValueOperand create_immediate_string(std::string_view value) const;
    ValueOperand create_immediate_real(double value) const;
    ValueOperand create_vector_operand() const;

    LabelOperand create_label() const;
    template<class F>
    inline Register create_variable(const std::string_view name, F create_register) {
        auto it = m_variables.find(name);
        if (it == m_variables.end()) {
            Register reg = create_register();
            m_variables.insert({name, reg});
            return reg;
        } else {
            return it->second;
        }
    }
    FunctionOperand create_function_from_id(unsigned) const;

    LabelOperand create_and_queue_label();

    // Add a label to waiting labels
    void queue_label(const LabelOperand &);

    // Binds the queued labels to the given quad. Also clears the label
    // queue.
    void bind_queued_labels(size_t);

    void dump_quads() const;

    // Returns the name bound to variable_id if present, otherwise
    // tmp#<variable_id>.
    std::string resolve_variable_name(unsigned) const;

    auto declaration() const { return m_decl; }

    const auto &quads() const { return m_quads; }
    auto &quads() { return m_quads; }

    unsigned id() const;

    // Return the index of the quad bound to label
    size_t quad_idx(const LabelOperand) const;

   private:
    // Bind the given label id to the given quad
    void bind_label(LabelOperand, size_t);

    const FunctionDecl *m_decl;

    std::vector<Quad> m_quads {};
    std::map<std::string_view, Register> m_variables {};
    std::map<LabelOperand, size_t> m_labels {};
    std::vector<LabelOperand> m_waiting_labels {};
};