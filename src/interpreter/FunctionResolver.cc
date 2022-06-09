#include "interpreter/FunctionResolver.hh"

#include "Constants.hh"

void FunctionResolver::add_function(FunctionOperand function, InstructionAddress addr) {
    ASSERT(m_address_map.find(function) == m_address_map.end());
    m_address_map.emplace(function, addr);
}

InstructionAddress FunctionResolver::resolve_function(FunctionOperand function) const {
  return m_address_map.at(function);
}
