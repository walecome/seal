#pragma once

#include <optional>
#include <variant>
#include "Constants.hh"
#include "Operand.hh"
#include "Register.hh"
#include "fmt/core.h"

class QuadSource {
   public:
    QuadSource() : m_source(std::nullopt) {}
    explicit QuadSource(ValueOperand value) : m_source(std::move(value)) {}
    explicit QuadSource(Register reg) : m_source(std::move(reg)) {}
    explicit QuadSource(FunctionOperand func) : m_source(std::move(func)) {}

    bool empty() const { return !holds_value(); }

    bool is_register() const { return is_a<Register>(); }

    bool is_value() const { return is_a<ValueOperand>(); }

    bool is_function() const { return is_a<FunctionOperand>(); }
    
    Register as_register() const { return get_as<Register>(); }

    ValueOperand as_value() const { return get_as<ValueOperand>(); }

    FunctionOperand as_function() const { return get_as<FunctionOperand>(); }
    
    std::string to_string() const {
        if (!holds_value()) {
            return "_";
        }
        if (is_register()) {
            return as_register().to_string();
        }
        if (is_value()) {
            return as_value().to_debug_string();
        }

        if (is_function()) {
            return Operand { as_function() }.to_debug_string();
        }

        ASSERT_NOT_REACHED();
    }

   private:
    bool holds_value() const { return m_source.has_value(); }

    template <class T>
    bool is_a() const {
        if (!holds_value()) {
            return false;
        }
        return std::holds_alternative<T>(m_source.value());
    }

    std::string get_held_variant_name() const {
        if (!holds_value()) {
            return "EMPTY";
        }
        auto unwrapped_source = m_source.value();
        if (std::holds_alternative<Register>(unwrapped_source)) {
            return get_typename<Register>();
        }
        if (std::holds_alternative<ValueOperand>(unwrapped_source)) {
            return get_typename<ValueOperand>();
        }
        if (std::holds_alternative<FunctionOperand>(unwrapped_source)) {
            return get_typename<FunctionOperand>();
        }
        ASSERT_NOT_REACHED();
    }

    template <class T>
    std::string get_typename() const {
        return "UNKNOWN";
    }

    template <>
    std::string get_typename<Register>() const {
        return "Register";
    }

    template <>
    std::string get_typename<ValueOperand>() const {
        return "ValueOperand";
    }

    template <>
    std::string get_typename<FunctionOperand>() const {
        return "FunctionOperand";
    }
    
    template <class T>
    T get_as() const {
        if (!is_a<T>()) {
            std::string error_message =
                fmt::format("Invalid get_as<{}>() on held type \"{}\"\n",
                            get_typename<T>(), get_held_variant_name());
            ASSERT_NOT_REACHED_MSG(error_message.c_str());
        }
        return std::get<T>(m_source.value());
    }

    const std::optional<
        std::variant<Register, ValueOperand, FunctionOperand>> m_source;
};
