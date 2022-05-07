#pragma once

#include <map>
#include <optional>
#include <stack>
#include <vector>
#include <queue>

#include "interpreter/StackFrame.hh"
#include "ir/Operand.hh"

class Quad;
class QuadDest;
class QuadSource;
class Register;
struct QuadCollection;

class Interpreter {
   public:
    Interpreter(const QuadCollection&, StringTable* string_table, bool verbose);

    void interpret();

    // FIXME: Should not point to an Operand, should point to some Value class.
    Operand resolve_register(Register reg) const;
    // FIXME: Should not point to an Operand, should point to some Value class.
    ValueOperand resolve_source(const QuadSource& source) const;
    void set_register(Register reg, Operand operand);

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
    void pop_arg(const Quad&);
    void save(const Quad&);
    void restore(const Quad&);
    void call(const Quad&);
    void call_c(const Quad&);
    void set_ret_type(const Quad&);
    void ret(const Quad&);
    void move(const Quad&);
    void index_move(const Quad&);
    void index_assign(const Quad&);

    void interpret_and(const Quad&);
    void interpret_or(const Quad&);
    
    unsigned resolve_label(const QuadDest& dest) const;
    // FIXME: Should no return a ValueOperand, should return some other Interpreter-specific Value wrapper.
    std::optional<ValueOperand> call_c_func(
        StringTable::Key lib,
        StringTable::Key func,
        const std::vector<ValueOperand>& args,
        unsigned return_type_id
    );

    StackFrame* current_frame();
    void enter_new_frame();
    void exit_frame();

    unsigned take_pending_type_id();
    void set_pending_type_id(unsigned value);

    const QuadCollection& m_quads;
    StringTable* m_string_table;
    // FIXME: Should not point to an Operand, should point to some Value class.
    std::vector<Operand> m_registers;
    bool m_verbose;
    std::stack<StackFrame> m_stack_frames {};
    std::queue<ArgumentWrapper> m_arguments { };
    // FIXME: Should not point to an Operand, should point to some Value class.
    std::stack<Operand> m_stack {};
    std::optional<unsigned> m_pending_return_type {};
};
