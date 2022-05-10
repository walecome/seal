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
    bool m_jump_performed { false };
};

