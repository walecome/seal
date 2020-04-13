#pragma once

#include <map>
#include <optional>
#include <vector>

#include "ir/Operand.hh"

class StackFrame {
   public:
    StackFrame() = delete;
    StackFrame(unsigned program_counter, StackFrame* parent)
        : m_program_counter { program_counter }, m_parent { parent } {}

    void set_variable(VariableOperand, ValueOperand, bool);
    ValueOperand get_variable(VariableOperand) const;

    // Decays an Operand to ValueOperand
    ValueOperand resolve_operand(Operand) const;

    inline unsigned program_counter() const { return m_program_counter; }
    inline void increment_program_counter() { ++m_program_counter; }
    inline void set_program_counter(unsigned value) {
        m_program_counter = value;
    }

    inline void set_parent_program_counter(unsigned value) {
        ASSERT(m_parent != nullptr);
        m_parent->set_program_counter(value);
    }

    std::optional<VariableOperand> return_variable() const {
        return m_return_variable;
    }
    void set_return_variable(VariableOperand value) {
        m_return_variable = std::move(value);
    }

    const std::map<std::string_view, ValueOperand>& get_variables() const {
        return m_variables;
    }

    bool jump_performed() const { return m_jump_performed; }
    void set_jump_performed(bool value) { m_jump_performed = value; }

    bool is_main_frame() const { return m_parent == nullptr; }

   private:
    // Assign a value to an existing variable.
    void assign_variable(VariableOperand var, ValueOperand value);

    // Return the inner most stack frame that holds the variable with name
    // identifier.
    StackFrame* get_variable_frame(VariableOperand identifier);

    unsigned m_program_counter;
    StackFrame* m_parent;
    std::map<std::string_view, ValueOperand> m_variables {};
    std::optional<VariableOperand> m_return_variable {};
    bool m_jump_performed { false };
};

class ArgumentWrapper {
   public:
    ArgumentWrapper() = default;

    void add_named_argument(VariableOperand, ValueOperand);
    void add_argument(ValueOperand);
    const std::vector<ValueOperand>& value_vector() const;

    template <class Callback>
    void for_each_name_value(Callback callback) {
        ASSERT(m_identifiers.size() == m_values.size());
        for (unsigned i = 0; i < m_identifiers.size(); ++i) {
            callback(m_identifiers[i], m_values[i]);
        }
    }

   private:
    std::vector<VariableOperand> m_identifiers {};
    std::vector<ValueOperand> m_values {};
};