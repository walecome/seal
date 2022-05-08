#include "interpreter/StackFrame.hh"

unsigned StackFrame::program_counter() const { return m_program_counter; }

void StackFrame::increment_program_counter() { ++m_program_counter; }

void StackFrame::set_program_counter(unsigned value) {
    m_program_counter = value;
}

void StackFrame::set_parent_program_counter(unsigned value) {
    ASSERT(m_parent != nullptr);
    m_parent->set_program_counter(value);
}

bool StackFrame::jump_performed() const { return m_jump_performed; }

void StackFrame::set_jump_performed(bool value) { m_jump_performed = value; }

bool StackFrame::is_main_frame() const { return m_parent == nullptr; }
