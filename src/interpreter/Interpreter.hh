#pragma once

#include <map>
#include <optional>
#include <stack>
#include <vector>

#include "interpreter/StackFrame.hh"
#include "ir/Operand.hh"

class Quad;
struct QuadCollection;

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
    void prepare_frame();

    StackFrame* current_frame();
    void enter_new_frame();
    void exit_frame();

    const QuadCollection& m_quads;

    std::stack<StackFrame> m_stack_frames {};
};