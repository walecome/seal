#pragma once

#include <optional>
#include <variant>
#include "Constants.hh"
#include "Operand.hh"
#include "Register.hh"

class QuadDest {
  public:
  QuadDest() : m_dest(std::nullopt) {}
  explicit QuadDest(Register reg) : m_dest(std::move(reg)) {}
  explicit QuadDest(LabelOperand label) : m_dest(std::move(label)) {}

  bool is_label() {
    return is_a<LabelOperand>();
  }
  
  bool is_register() {
    return is_a<Register>();
  }

  LabelOperand as_label() {
    return get_as<LabelOperand>();
  }
  
  Register as_register() {
    return get_as<Register>();
  }

  private:

  bool holds_value() const {
    return m_dest.has_value();
  }

  template<class T>
  bool is_a() const {
    if (!holds_value()) {
      return false;
    }
    return std::holds_alternative<T>(m_dest.value());
  }
  
  template<class T>
  T get_as() {
    ASSERT(is_a<T>());
    return std::get<T>(m_dest.value());
  }

  const std::optional<std::variant<Register, LabelOperand>> m_dest;
};