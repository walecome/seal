#pragma once

#include <functional>
#include <map>
#include <optional>
#include <queue>
#include <stack>
#include <vector>

#include "interpreter/Context.hh"
#include "ir/Operand.hh"

#include "interpreter/InstructionAddress.hh"
#include "interpreter/ValueFactory.hh"
#include "interpreter/RegisterWindow.hh"

class InstructionSequencer;
class Quad;
class Register;
class LabelResolver;
class FunctionResolver;

class Interpreter {
   public:
    Interpreter(InstructionSequencer* instruction_sequencer,
                const ValuePool* constant_pool,
                const LabelResolver* label_resolver,
                const FunctionResolver* function_resolver, bool verbose);

    void interpret();

    Value resolve_register(Register reg) const;
    Value resolve_to_value(const Operand& source) const;
    void set_register(Register reg, Value value);
    void set_register(Register reg, ptr_t<ValueFactory>&& value_factory);

   private:
    void interpret_quad(const Quad&);

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
    void call(const Quad&);
    void call_c(const Quad&);
    void set_ret_type(const Quad&);
    void ret(const Quad&);
    void move(const Quad&);
    void index_move(const Quad&);
    void index_assign(const Quad&);

    void interpret_and(const Quad&);
    void interpret_or(const Quad&);

    void compare(
        const Quad&,
        std::function<bool(const Value&, const Value&)> comparison_predicate);

    std::optional<ptr_t<ValueFactory>> call_c_func(
        std::string_view lib, std::string_view func,
        const std::vector<Value>& args, unsigned return_type_id);

    unsigned take_pending_type_id();
    void set_pending_type_id(unsigned value);

    Context& context();
    const Context& context() const;
    InstructionSequencer& sequencer();
    RegisterWindow& current_register_window();
    RegisterWindow& current_register_window() const;
    const LabelResolver& label_resolver() const;
    const FunctionResolver& function_resolver() const;

    bool is_main_function();

    void handle_crash();

    InstructionSequencer* m_instruction_sequencer;
    std::stack<RegisterWindow> m_register_windows;
    const LabelResolver* m_label_resolver;
    const FunctionResolver* m_function_resolver;
    bool m_verbose;
    Context m_context;
    std::queue<Value> m_arguments {};
    std::optional<unsigned> m_pending_return_type {};
};
