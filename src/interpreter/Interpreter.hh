#pragma once

#include <map>
#include <stack>
#include <vector>

#include "ir/Operand.hh"

class Quad;
struct QuadCollection;

class StackFrame {
   public:
    StackFrame() = default;
    StackFrame(StackFrame* parent) : m_parent { parent } {}

    void set_variable(Operand var, ValueOperand value);
    ValueOperand get_variable(VariableOperand var) const;

    // Decays an Operand to ValueOperand
    ValueOperand resolve_operand(Operand operand) const;

    void push_argument(ValueOperand value);
    void clear_arguments();
    const std::vector<ValueOperand>& get_arguments() const;

    unsigned program_counter() const { return m_current_quad_idx; }
    void increment_program_counter() { ++m_current_quad_idx; }
    void set_program_counter(unsigned value) { m_current_quad_idx = value; }

   private:
    // Assign a value to an existing variable.
    void assign_variable(VariableOperand var, ValueOperand value);

    // Return the inner most stack frame that holds the variable with name
    // identifier.
    StackFrame* get_variable_frame(VariableOperand identifier);

    StackFrame* m_parent { nullptr };
    std::map<std::string_view, ValueOperand> m_variables {};
    std::vector<ValueOperand> m_arguments {};
    unsigned m_current_quad_idx {};
};

class Interpreter {
   public:
    Interpreter(const QuadCollection&);

    void interpret();

   private:
    void interpret_function(unsigned function_id);

    // Interpret function for different opcodes
    void add(Quad);
    void sub(Quad);
    void mult(Quad);
    void div(Quad);
    void cmp_eq(Quad);
    void cmp_gt(Quad);
    void cmp_lt(Quad);
    void cmp_gteq(Quad);
    void cmp_lteq(Quad);
    void cmp_noteq(Quad);
    void jmp(Quad);
    void jmp_z(Quad);
    void jmp_nz(Quad);
    void push_arg(Quad);
    void call(Quad);
    void ret(Quad);
    void move(Quad);
    void index_move(Quad);

    StackFrame* current_frame() { return &m_stack_frames.top(); }
    void enter_new_frame() {
        m_stack_frames.push(StackFrame { current_frame() });
    };

    const QuadCollection& m_quads;

    std::stack<StackFrame> m_stack_frames {};

    bool m_jump_performed { false };

    size_t m_current_quad_idx;
};