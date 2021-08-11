#pragma once

#include <optional>
#include <variant>
#include "Constants.hh"
#include "Operand.hh"
#include "Register.hh"

class QuadSource {
  public:
  QuadSource() : m_source(std::nullopt) {}
  explicit QuadSource(ValueOperand value) : m_source(std::move(value)) {}
  explicit QuadSource(Register reg) : m_source(std::move(reg)) {}
  explicit QuadSource(FunctionOperand func) : m_source(std::move(func)) {}

  bool is_register() {
    return is_a<Register>();
  }
  
  bool is_value() {
    return is_a<ValueOperand>();
  }
  
  bool is_function() {
    return is_a<FunctionOperand>();
  }
  
  Register as_register() {
    return get_as<Register>();
  }

  ValueOperand as_value() {
    return get_as<ValueOperand>();
  }
  
  FunctionOperand as_function() {
    return get_as<FunctionOperand>();
  }
  
  private:

  bool holds_value() const {
    return m_source.has_value();
  }

  template<class T>
  bool is_a() const {
    if (!holds_value()) {
      return false;
    }
    return std::holds_alternative<T>(m_source.value());
  }
  
  template<class T>
  T get_as() {
    ASSERT(is_a<T>());
    return std::get<T>(m_source.value());
  }

  const std::optional<std::variant<Register, ValueOperand, FunctionOperand>> m_source;
};