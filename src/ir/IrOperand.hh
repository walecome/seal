#pragma once

#include <iterator>
#include <optional>
#include <vector>

#include "Constants.hh"
#include "common/ConstantPool.hh"
#include "common/Register.hh"

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
ADD_OPERAND_TYPE(BuiltinFunctionOperand, unsigned)

using operand_type_t =
    std::variant<ConstantPool::Entry, LabelOperand, BuiltinFunctionOperand,
                 FunctionOperand, Register>;

class IrFunction;

class IrOperand {
   public:
    explicit IrOperand(operand_type_t data) : m_data { data } {
    }

    static IrOperand empty() {
        return IrOperand();
    }

    bool is_constant_entry() const;
    bool is_label() const;
    bool is_function() const;
    bool is_builtin_function() const;
    bool is_register() const;

    bool is_used() const {
        return m_data.has_value();
    }

    auto env() const {
        return m_env;
    }

    void set_env(const IrFunction* env) {
        m_env = env;
    }

    std::string to_debug_string() const;

    ConstantPool::Entry as_constant_entry() const;
    LabelOperand as_label() const;
    FunctionOperand as_function() const;
    BuiltinFunctionOperand as_builtin_function() const;
    Register as_register() const;

   private:
    IrOperand() : m_data(std::nullopt) {
    }

    template <class T>
    bool holds() const {
        if (!m_data.has_value()) {
            return false;
        }
        return std::holds_alternative<T>(m_data.value());
    }

    std::optional<operand_type_t> m_data;

    const IrFunction* m_env { nullptr };
};
