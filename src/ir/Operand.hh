#pragma once

#include <iterator>
#include <variant>
#include <vector>

#include "Constants.hh"
#include "ir/Register.hh"
#include "common/PoolEntry.hh"

// This defines a struct with the identifier name. In addition it adds a member
// named value of the given type, as well as an overload for that type.
#define ADD_OPERAND_TYPE(NAME, TYPE) \
    struct NAME {                    \
        NAME() = default;            \
        TYPE value {};               \
        operator TYPE() const {      \
            return value;            \
        }                            \
    };

// For named operands (label, variable, function)
ADD_OPERAND_TYPE(LabelOperand, unsigned)
ADD_OPERAND_TYPE(FunctionOperand, unsigned)

using operand_type_t =
    std::variant<PoolEntry, LabelOperand, FunctionOperand, Register>;

class IrFunction;

class Operand {
   public:
    Operand() : m_used { false }, m_data {} {
    }
    explicit Operand(operand_type_t data) : m_used { true }, m_data { data } {
    }

    static Operand create_empty() {
        return Operand();
    }

    bool is_value_entry() const;
    bool is_label() const;
    bool is_function() const;
    bool is_register() const;

    bool is_used() const {
        return m_used;
    }

    auto env() const {
        return m_env;
    }

    void set_env(const IrFunction* env) {
        m_env = env;
    }

    std::string to_debug_string() const;

    PoolEntry as_value_entry() const;
    LabelOperand as_label() const;
    FunctionOperand as_function() const;
    Register as_register() const;

   private:
    template <class T>
    bool holds() const {
        return std::holds_alternative<T>(m_data);
    }

    bool m_used;
    operand_type_t m_data;

    const IrFunction* m_env { nullptr };
};
