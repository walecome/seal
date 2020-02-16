#pragma once

#include <map>
#include <stack>

#include "ir/Operand.hh"

class IrProgram;
class IrFunction;
class Quad;

class StackFrame {
   public:
    StackFrame();
    StackFrame(StackFrame* parent) : m_parent { parent } {}

    void set_variable(const std::string_view identifier, const Operand value);
    Operand get_variable(const std::string_view identifier);

   private:
    // Assign a value to an existing variable.
    void assign_variable(const std::string_view identifier,
                         const Operand value);

    // Return the inner most stack frame that holds the variable with name
    // identifier.
    StackFrame* get_variable_frame(std::string_view identifier);

    StackFrame* m_parent { nullptr };
    std::map<std::string_view, Operand> m_variables {};
};

class Interpreter {
   public:
    Interpreter(const IrProgram* ir_program) : m_ir_program { ir_program } {}

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

    StackFrame& current_frame() { return m_stack_frames.top(); }

    const IrProgram* m_ir_program;
    std::stack<StackFrame> m_stack_frames {};
};