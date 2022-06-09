#include "interpreter/LabelResolver.hh"

void LabelResolver::add_label(LabelOperand label, InstructionAddress addr) {
    ASSERT(m_address_map.find(label) == m_address_map.end());
    m_address_map.emplace(label, addr);
}

InstructionAddress LabelResolver::resolve_label(LabelOperand label) const {
  return m_address_map.at(label);
}
