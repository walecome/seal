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

    unsigned program_counter() const;
    void increment_program_counter();
    void set_program_counter(unsigned value);
    void set_parent_program_counter(unsigned value);

    bool jump_performed() const;
    void set_jump_performed(bool value);

    bool is_main_frame() const;

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
