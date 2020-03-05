#pragma once

#include <map>
#include <stack>
#include <vector>

#include "ir/Operand.hh"

class IrProgram;
class IrFunction;
class Quad;

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

   private:
    // Assign a value to an existing variable.
    void assign_variable(VariableOperand var, ValueOperand value);

    // Return the inner most stack frame that holds the variable with name
    // identifier.
    StackFrame* get_variable_frame(VariableOperand identifier);

    StackFrame* m_parent { nullptr };
    std::map<VariableOperand, ValueOperand> m_variables {};
    std::vector<ValueOperand> m_arguments {};
};

class Interpreter {
   public:
    Interpreter(const IrProgram* ir_program) : m_ir_program { ir_program } {
        m_stack_frames.push(StackFrame {});
    }

    void interpret();

   private:
    void interpret_function(const IrFunction* function);

    // Interpret function for different opcodes
    void add(const Quad* quad);
    void sub(const Quad* quad);
    void mult(const Quad* quad);
    void div(const Quad* quad);
    void cmp_eq(const Quad* quad);
    void cmp_gt(const Quad* quad);
    void cmp_lt(const Quad* quad);
    void cmp_gteq(const Quad* quad);
    void cmp_lteq(const Quad* quad);
    void cmp_noteq(const Quad* quad);
    void jmp(const Quad* quad);
    void jmp_z(const Quad* quad);
    void jmp_nz(const Quad* quad);
    void push_arg(const Quad* quad);
    void call(const Quad* quad);
    void ret(const Quad* quad);
    void move(const Quad* quad);
    void index_move(const Quad* quad);

    StackFrame* current_frame() { return &m_stack_frames.top(); }
    void enter_new_frame() {
        m_stack_frames.push(StackFrame { current_frame() });
    };

    const IrProgram* m_ir_program;
    std::stack<StackFrame> m_stack_frames {};
    std::stack<const IrFunction*> m_call_stack {};

    bool m_jump_performed { false };

    size_t m_current_quad_idx;
};