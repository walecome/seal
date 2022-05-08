#pragma once

#include <map>
#include <optional>
#include <vector>

#include "Constants.hh"

#include "interpreter/Value.hh"

class StackFrame {
   public:
    StackFrame() = delete;
    StackFrame(unsigned program_counter, StackFrame* parent)
        : m_program_counter { program_counter }, m_parent { parent } {}

    inline unsigned program_counter() const { return m_program_counter; }
    inline void increment_program_counter() { ++m_program_counter; }
    inline void set_program_counter(unsigned value) {
        m_program_counter = value;
    }

    inline void set_parent_program_counter(unsigned value) {
        ASSERT(m_parent != nullptr);
        m_parent->set_program_counter(value);
    }

    std::optional<Value> return_value() const {
        return m_return_variable;
    }

    void set_return_value(Value value) {
        m_return_variable = std::move(value);
    }

    bool jump_performed() const { return m_jump_performed; }
    void set_jump_performed(bool value) { m_jump_performed = value; }

    bool is_main_frame() const { return m_parent == nullptr; }

   private:
    unsigned m_program_counter;
    StackFrame* m_parent;
    std::map<std::string_view, ValueOperand> m_variables {};
    std::optional<VariableOperand> m_return_variable {};
    bool m_jump_performed { false };
};

struct ArgumentWrapper {
    ArgumentWrapper() = delete;
    explicit ArgumentWrapper(ValueOperand value)
        : identifier(std::nullopt), value(std::move(value)) {}
    explicit ArgumentWrapper(std::string_view identifier, ValueOperand value)
        : identifier(identifier), value(std::move(value)) {}

    const std::optional<std::string_view> identifier;
    const Value value;
};
