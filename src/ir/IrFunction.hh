#pragma once

#include <map>
#include <optional>
#include <set>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Constants.hh"
#include "Operand.hh"
#include "Quad.hh"
#include "Register.hh"

class FunctionDecl;

class IrFunction {
    MAKE_NONCOPYABLE(IrFunction)
    MAKE_NONMOVABLE(IrFunction)

   public:
    IrFunction(const FunctionDecl *decl) : m_decl { decl } {
    }

    // Construct a quad with the given opcode and operands, placing it in
    // the m_quads vector. Also binds any queued lables to the quad and
    // clears the label queue.
    void add_quad(OPCode, Operand, Operand, Operand);

    LabelOperand create_label() const;

    Register create_register();
    Register create_register_for_identifier(std::string_view identifier);
    std::optional<Register> find_register_for_identifier(std::string_view identifier);

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

    const FunctionDecl *declaration() const {
        return m_decl;
    }

    template <class F>
    void for_each_quad(F callback) {
        for (const ptr_t<Quad> &quad_ptr : m_quads) {
            callback(quad_ptr.get());
        }
    }

    const Quad& get_last_quad() const;

    unsigned id() const;

    // Return the index of the quad bound to label
    size_t quad_idx(const LabelOperand) const;

    LabelOperand get_epilogue_label() const {
        return m_epilogue_label;
    }

    void enter_block();
    void exit_block();

   private:
    // Bind the given label id to the given quad
    void bind_label(LabelOperand, size_t);
    Register allocate_register();

    std::map<std::string_view, Register>& current_block_variables();

    const FunctionDecl *m_decl;
    const LabelOperand m_epilogue_label { create_label() };

    std::vector<ptr_t<Quad>> m_quads {};
    std::vector<std::map<std::string_view, Register>> m_variables {};
    std::map<LabelOperand, size_t> m_labels {};
    std::vector<LabelOperand> m_waiting_labels {};
    // TODO: Change this to 0 when we have special register window for return register.
    size_t m_register_count { 1 };
};
