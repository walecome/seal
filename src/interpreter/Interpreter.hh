#pragma once

#include <map>
#include <stack>

class IrProgram;
class IrFunction;
class Quad;

class StackFrame {
   public:
   private:
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