#pragma once

#include <iterator>
#include <variant>
#include <vector>

#include "Constants.hh"
#include "OperandType.hh"
#include "ir/Register.hh"
#include "utility/StringTable.hh"


// This defines a struct with the identifier name. In addition it adds a member
// named value of the given type, as well as an overload for that type.
#define ADD_OPERAND_TYPE(NAME, TYPE)                                         \
    struct NAME {                                                            \
        NAME() = default;                                                    \
        TYPE value {};                                                       \
        operator TYPE() const { return value; }                              \
    };

// For immediate operands
ADD_OPERAND_TYPE(IntOperand, unsigned long)
ADD_OPERAND_TYPE(RealOperand, double)

struct ValueOperand;

struct VectorOperand {
   private:
    using vector_type_t = std::vector<ValueOperand>;

   public:
    using value_type_t = std::shared_ptr<vector_type_t>;
    VectorOperand() : value { std::make_shared<vector_type_t>() } {}

    VectorOperand copy() {
        VectorOperand copy {};
        std::copy(value->begin(), value->end(), std::back_inserter(*copy.value));
        return copy;
    }

    value_type_t value;

    operator value_type_t() const { return value; }
};

struct StringOperand {
    StringOperand() = default;
    explicit StringOperand(const StringTable::Key key, StringTable* string_table) : value(key), m_string_table(string_table) {}

    StringOperand(const StringOperand& other) {
        value = other.value;
        m_string_table = other.m_string_table;
    }

    StringTable::Key value;

    StringOperand& operator=(const StringOperand& other) {
        value = StringTable::Key::from(other.value.id);
        m_string_table = other.m_string_table;
        return *this;
    }

    operator StringTable::Key() const { return value; }

    StringTable::value_type_t resolve() const {
        return m_string_table->get_at(value);
    }

    private:
        StringTable* m_string_table {nullptr};
};

using value_operand_t =
    std::variant<IntOperand, RealOperand, StringOperand, VectorOperand>;

struct ValueOperand {
    value_operand_t value {};

    VectorOperand as_vector() const;
    IntOperand    as_int() const;
    RealOperand   as_real() const;
    StringOperand as_string() const;

    bool is_vector() const;
    bool is_integer() const;
    bool is_number() const;
    bool is_real() const;
    bool is_string() const;

    template <class T>
    bool holds() const {
        return std::holds_alternative<T>(value);
    }

    template <class T>
    T get_as() const {
        return std::get<T>(value);
    }
    
    std::string to_debug_string() const;
    std::string to_value_string() const;
};

// For named operands (label, variable, function)
ADD_OPERAND_TYPE(LabelOperand, unsigned)
ADD_OPERAND_TYPE(VariableOperand, std::string_view)
ADD_OPERAND_TYPE(FunctionOperand, unsigned)

using operand_type_t =
    std::variant<ValueOperand, LabelOperand, VariableOperand, FunctionOperand, Register>;

class IrFunction;

class Operand {
   public:
    Operand() : m_used { false }, m_data {} {}
    explicit Operand(operand_type_t data) : m_used { true }, m_data { data } {}

    static Operand create_empty() {
      return Operand();
    }

    bool is_value() const;
    bool is_label() const;
    bool is_variable() const;
    bool is_function() const;
    bool is_register() const;

    bool is_used() const { return m_used; }

    auto env() const { return m_env; }
    void set_env(const IrFunction* env) { m_env = env; }

    std::string to_debug_string() const;
    std::string to_value_string() const;

    ValueOperand as_value() const;
    VariableOperand as_variable() const;
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

