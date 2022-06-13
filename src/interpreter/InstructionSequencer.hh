#pragma once

#include <optional>
#include <stack>
#include <vector>

#include "interpreter/InstructionAddress.hh"

class RelocatedQuad;
class InstructionSequencer {
   public:
    InstructionSequencer(std::vector<RelocatedQuad>&& quads,
                         InstructionAddress start_address);

    const RelocatedQuad& fetch_next_instruction();

    void set_jump_address(InstructionAddress addr);

    void call(InstructionAddress function_addr);
    void ret();

    void dump() const;

    bool is_in_main_function() const;

   private:
    bool get_and_set_started();
    void maybe_advance_program_counter();

    const std::vector<RelocatedQuad> m_quads;
    InstructionAddress m_program_counter;
    std::optional<InstructionAddress> m_jump_address;
    bool m_has_started { false };
    std::stack<InstructionAddress> m_call_stack {};
};
