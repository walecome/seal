#pragma once

#include <cstddef>

#include "common/Register.hh"

class Value;

class RegisterWindow {
  public:
    RegisterWindow() = delete;
    explicit RegisterWindow(size_t register_count);

    void set_register_value(Register reg, Value value);
    Value get_from_register(Register reg) const;

  private:
    std::vector<Value> m_registers;
};
