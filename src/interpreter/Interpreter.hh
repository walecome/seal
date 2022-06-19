#pragma once

#include <functional>
#include <map>
#include <optional>
#include <queue>
#include <stack>
#include <vector>

#include "common/RelocatedQuad.hh"

#include "interpreter/InstructionAddress.hh"
#include "interpreter/RegisterWindow.hh"

class InstructionSequencer;
class Register;
class ConstantPool;

class Interpreter {
   public:
    Interpreter(InstructionSequencer* instruction_sequencer,
                const ConstantPool* constant_pool, bool verbose);

    void interpret();

    Value resolve_register(Register reg) const;
    Value resolve_to_value(const Operand& source) const;
    void set_register(Register reg, Value value);

   private:
    void interpret_quad(const RelocatedQuad&);

    // Interpret function for different opcodes
    void add(const RelocatedQuad&);
    void sub(const RelocatedQuad&);
    void mult(const RelocatedQuad&);
    void div(const RelocatedQuad&);
    void cmp_eq(const RelocatedQuad&);
    void cmp_gt(const RelocatedQuad&);
    void cmp_lt(const RelocatedQuad&);
    void cmp_gteq(const RelocatedQuad&);
    void cmp_lteq(const RelocatedQuad&);
    void cmp_noteq(const RelocatedQuad&);
    void jmp(const RelocatedQuad&);
    void jmp_z(const RelocatedQuad&);
    void jmp_nz(const RelocatedQuad&);
    void push_arg(const RelocatedQuad&);
    void pop_arg(const RelocatedQuad&);
    void call(const RelocatedQuad&);
    void call_c(const RelocatedQuad&);
    void set_ret_type(const RelocatedQuad&);
    void ret(const RelocatedQuad&);
    void move(const RelocatedQuad&);
    void index_move(const RelocatedQuad&);
    void index_assign(const RelocatedQuad&);

    void interpret_and(const RelocatedQuad&);
    void interpret_or(const RelocatedQuad&);

    void alloc_regs(const RelocatedQuad&);
    void vec_create(const RelocatedQuad&);
    void vec_add(const RelocatedQuad&);

    void compare(
        const RelocatedQuad&,
        std::function<bool(const Value&, const Value&)> comparison_predicate);

    std::optional<Value> call_c_func(std::string_view lib,
                                     std::string_view func,
                                     const std::vector<Value>& args,
                                     unsigned return_type_id);

    unsigned take_pending_type_id();
    void set_pending_type_id(unsigned value);

    InstructionSequencer& sequencer();
    const ConstantPool& constant_pool() const;
    RegisterWindow& current_register_window();
    const RegisterWindow& current_register_window() const;

    void handle_crash();

    InstructionSequencer* m_instruction_sequencer;
    const ConstantPool* m_constant_pool;
    std::stack<RegisterWindow> m_register_windows;
    bool m_verbose;
    std::queue<Value> m_arguments {};
    std::optional<unsigned> m_pending_return_type {};
};
