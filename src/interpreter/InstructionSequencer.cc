#include "interpreter/InstructionSequencer.hh"

#include "Constants.hh"
#include "ir/Quad.hh"

InstructionSequencer::InstructionSequencer(std::vector<Quad>&& quads,
                                           InstructionAddress start_address)
    : m_quads(quads), m_program_counter(start_address) {
}

const Quad& InstructionSequencer::fetch_next_instruction() {
    maybe_advance_program_counter();

    size_t index = m_program_counter.unwrap();
    ASSERT(index < m_quads.size());

    return m_quads[index];
}

void InstructionSequencer::set_jump_address(InstructionAddress addr) {
    ASSERT(!m_jump_address.has_value());
    m_jump_address = addr;
}

void InstructionSequencer::call(InstructionAddress addr) {
    // Return address points to the address after the call
    InstructionAddress return_address = m_program_counter.increment();
    m_call_stack.push(return_address);
    set_jump_address(addr);
}

void InstructionSequencer::ret() {
    ASSERT(!m_call_stack.empty());
    InstructionAddress return_address = m_call_stack.top();
    m_call_stack.pop();
    set_jump_address(return_address);
}

void InstructionSequencer::dump() const {
    ASSERT_NOT_REACHED_MSG("InstructionSequencer::dump() not implemented");
}

bool InstructionSequencer::is_in_main_function() const {
    return m_call_stack.empty();
}

bool InstructionSequencer::get_and_set_started() {
    m_has_started = true;
    return m_has_started;
}

void InstructionSequencer::maybe_advance_program_counter() {
    // Need to start with the starting address
    if (!get_and_set_started()) {
        return;
    }

    if (m_jump_address.has_value()) {
        m_program_counter = m_jump_address.value();
        m_jump_address = std::nullopt;
    } else {
        m_program_counter = m_program_counter.increment();
    }
}
