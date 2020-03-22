#pragma once

#include <map>
#include <unordered_map>
#include <vector>

#include "Constants.hh"
#include "Quad.hh"

class FunctionDecl;

class BasicBlock {
   public:
    BasicBlock();

    void add_quad(Quad);
    void add_parent(BasicBlock *);

    void print_quads() const;
    std::string name() const;
    const std::vector<BasicBlock *> &parents() const;

    Quad &back() { return m_quads.back(); }

    void assign_variable(VariableOperand, std::string_view);

    unsigned get_variable(std::string_view) const;

   private:
    unsigned m_id;
    std::vector<BasicBlock *> m_parents {};
    std::vector<Quad> m_quads {};

    std::unordered_map<std::string_view, unsigned> m_varname_to_id {};
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
    void queue_label(const Operand &);
    void bind_queued_labels(size_t);
    void bind_variable(const Operand &, const std::string_view);
    void print_blocks() const;
    std::string_view resolve_variable_name(unsigned) const;
    auto declaration() const { return m_decl; }
    unsigned id() const;
    size_t quad_idx(const LabelOperand) const;

    BasicBlock *current_block() { return m_basic_blocks.back().get(); }
    BasicBlock *current_block() const { return m_basic_blocks.back().get(); }

    BasicBlock *new_basic_block(BasicBlock * = nullptr);
    BasicBlock *new_basic_block(std::vector<BasicBlock *> &);

   private:
    Operand create_variable_from_id(unsigned) const;

    void bind_label(LabelOperand, size_t);

    const FunctionDecl *m_decl;

    std::vector<ptr_t<BasicBlock>> m_basic_blocks {};
    std::map<LabelOperand, size_t> m_labels {};
    std::vector<LabelOperand> m_waiting_labels {};
    std::unordered_map<unsigned, std::string_view> m_variable_ref {};

    std::unordered_map<std::string_view, std::vector<BasicBlock *>>
        m_var_to_blocks {};
};