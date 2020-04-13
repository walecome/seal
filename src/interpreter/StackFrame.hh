#pragma once

#include <map>
#include <optional>
#include <vector>

#include "ir/Operand.hh"

class StackFrame {
   public:
    StackFrame() = default;
    StackFrame(StackFrame* parent) : m_parent { parent } {}

    void set_variable(VariableOperand, ValueOperand, bool);
    ValueOperand get_variable(VariableOperand) const;

    // Decays an Operand to ValueOperand
    ValueOperand resolve_operand(Operand) const;

    inline unsigned program_counter() const { return m_current_quad_idx; }
    inline void increment_program_counter() { ++m_current_quad_idx; }
    inline void set_program_counter(unsigned value) {
        m_current_quad_idx = value;
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

   private:
    // Assign a value to an existing variable.
    void assign_variable(VariableOperand var, ValueOperand value);

    // Return the inner most stack frame that holds the variable with name
    // identifier.
    StackFrame* get_variable_frame(VariableOperand identifier);

    StackFrame* m_parent { nullptr };
    std::map<std::string_view, ValueOperand> m_variables {};
    unsigned m_current_quad_idx {};
    std::optional<VariableOperand> m_return_variable {};
    bool m_jump_performed { false };
};
