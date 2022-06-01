#pragma once

#include <optional>

class Quad;

#include "interpreter/InstructionAddress.hh"

class InstructionSequencer {
  public:
    const Quad& fetch_next_instruction();

    void set_jump_address(InstructionAddress addr);

    void call(InstructionAddress function_addr);
    void ret();

    void dump() const;

  private:
    InstructionAddress m_program_counter;
    std::optional<InstructionAddress> m_jump_address;
};

