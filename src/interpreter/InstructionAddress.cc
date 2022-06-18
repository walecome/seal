#include "interpreter/InstructionAddress.hh"

#include <fmt/format.h>

InstructionAddress::InstructionAddress() = default;

InstructionAddress::InstructionAddress(size_t addr) : m_addr(addr) {
}

InstructionAddress InstructionAddress::increment() const {
    return InstructionAddress(m_addr + 1);
}

size_t InstructionAddress::unwrap() const {
    return m_addr;
}

std::string InstructionAddress::as_hex() const {
  return fmt::format("{0:#x}", m_addr);
}
