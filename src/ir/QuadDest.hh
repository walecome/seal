#pragma once

#include <optional>
#include <variant>
#include "Constants.hh"
#include "Operand.hh"
#include "Register.hh"
#include "fmt/core.h"

class QuadDest {
  public:
  QuadDest() : m_dest(std::nullopt) {}
  explicit QuadDest(Register reg) : m_dest(std::move(reg)) {}
  explicit QuadDest(LabelOperand label) : m_dest(std::move(label)) {}

  bool is_label() const {
    return is_a<LabelOperand>();
  }
  
  bool is_register() const {
    return is_a<Register>();
  }

  LabelOperand as_label() const {
    return get_as<LabelOperand>();
  }
  
  Register as_register() const {
    return get_as<Register>();
  }
  
  bool holds_value() const {
    return m_dest.has_value();
  }

  std::string to_string() const {
    if (!holds_value()) {
      return "_";
    }
    if (is_label()) {
      return Operand{as_label()}.to_debug_string();
    }
    if (is_register()) {
      return as_register().to_string();
    }

    ASSERT_NOT_REACHED();
  }

  private:


  template<class T>
  bool is_a() const {
    if (!holds_value()) {
      return false;
    }
    return std::holds_alternative<T>(m_dest.value());
  }
  
  template<class T>
  T get_as() const {
    ASSERT(is_a<T>());
    return std::get<T>(m_dest.value());
  }

  const std::optional<std::variant<Register, LabelOperand>> m_dest;
};
