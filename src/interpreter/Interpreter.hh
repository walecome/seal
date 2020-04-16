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
    Interpreter(const QuadCollection&, bool verbose);

    void interpret();

   private:
    void interpret_function(unsigned function_id);

    // Interpret function for different opcodes
    void add(const Quad&);
    void sub(const Quad&);
    void mult(const Quad&);
    void div(const Quad&);
    void cmp_eq(const Quad&);
    void cmp_gt(const Quad&);
    void cmp_lt(const Quad&);
    void cmp_gteq(const Quad&);
    void cmp_lteq(const Quad&);
    void cmp_noteq(const Quad&);
    void jmp(const Quad&);
    void jmp_z(const Quad&);
    void jmp_nz(const Quad&);
    void push_arg(const Quad&);
    void call(const Quad&);
    void ret(const Quad&);
    void move(const Quad&);
    void index_move(const Quad&);

    StackFrame* current_frame();
    void enter_new_frame();
    void exit_frame();

    ArgumentWrapper take_arguments();

    const QuadCollection& m_quads;
    bool m_verbose;
    std::stack<StackFrame> m_stack_frames {};
    std::optional<ArgumentWrapper> m_arguments { std::nullopt };
};