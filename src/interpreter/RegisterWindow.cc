#include "interpreter/RegisterWindow.hh"

#include "common/Value.hh"
#include "Constants.hh"

RegisterWindow::RegisterWindow(size_t register_count)
    : m_registers(std::vector<Value>(register_count)) {
}

void RegisterWindow::set_register_value(Register reg, Value value) {
  ASSERT(m_registers.size() < reg.index());
  m_registers[reg.index()] = value;
}

Value RegisterWindow::get_from_register(Register reg) const {
  ASSERT(m_registers.size() < reg.index());
  return m_registers[reg.index()];
}
