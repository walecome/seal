#pragma once

#include <map>
#include <unordered_map>
#include <vector>

#include "Constants.hh"
#include "Quad.hh"

class FunctionDecl;

class BasicBlock {
   public:
    BasicBlock() = default;

    void add_quad(Quad);
    void add_child(BasicBlock *);

    void print_quads() const;

    Quad &back() { return m_quads.back(); }

   private:
    std::vector<BasicBlock *> m_children {};
    std::vector<Quad> m_quads {};
};

class IrFunction {
    MAKE_NONCOPYABLE(IrFunction)
    MAKE_NONMOVABLE(IrFunction)

   public:
    IrFunction(FunctionDecl *decl) : m_decl { decl } {}

    Operand create_immediate_int(unsigned long) const;
    Operand create_immediate_string(std::string_view) const;
    Operand create_immediate_real(double) const;

    Operand create_label() const;
    Operand create_variable() const;
    Operand create_function_from_id(unsigned) const;

    Operand get_variable(std::string_view) const;

    Operand create_and_queue_label();

    void construct_quad(OPCode, Operand, Operand, Operand);

    // Add a label to waiting labels
    void queue_label(const Operand &);

    // Binds the queued labels to the given quad. Also clears the label queue.
    void bind_queued_labels(size_t);

    // Bind a source code variable name to an IR variable
    void bind_variable(const Operand &, const std::string_view);

    void print_blocks() const;

    // Returns the name bound to variable_id if present, otherwise
    // tmp#<variable_id>.
    std::string_view resolve_variable_name(unsigned) const;

    auto declaration() const { return m_decl; }

    unsigned id() const;

    // Return the index of the quad bound to label
    size_t quad_idx(const LabelOperand) const;

    BasicBlock &current_block() { return m_basic_blocks.back(); }

    void new_basic_block();

   private:
    Operand create_variable_from_id(unsigned) const;

    void bind_label(LabelOperand, size_t);

    const FunctionDecl *m_decl;

    std::vector<BasicBlock> m_basic_blocks {};
    std::map<LabelOperand, size_t> m_labels {};
    std::vector<LabelOperand> m_waiting_labels {};
    std::unordered_map<unsigned, std::string_view> m_variable_ref {};
    std::unordered_map<std::string_view, unsigned> m_varname_to_id {};
};