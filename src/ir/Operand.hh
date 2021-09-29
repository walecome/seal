#pragma once

#include <variant>
#include <vector>

#include "Constants.hh"
#include "OperandType.hh"
#include "utility/StringTable.hh"

// This defines a struct with the identifier name. In addition it adds a member
// named value of the given type, as well as an overload for that type.
#define ADD_OPERAND_TYPE(NAME, TYPE)                                         \
    struct NAME {                                                            \
        NAME() = default;                                                    \
        TYPE value {};                                                       \
        operator TYPE() const { return value; }                              \
        friend std::ostream& operator<<(std::ostream& os, const NAME& cls) { \
            os << cls.value;                                                 \
            return os;                                                       \
        }                                                                    \
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

    value_type_t value;

    operator value_type_t() const { return value; }
};

struct StringOperand {
    StringOperand() : value(StringTable::Key::from(0)) {}
    explicit StringOperand(const StringTable::Key key) : value(key) {}
    const StringTable::Key value;

    operator StringTable::Key() const { return value; }
};

using value_operand_t =
    std::variant<IntOperand, RealOperand, StringOperand, VectorOperand>;

struct ValueOperand {
    value_operand_t value {};

    VectorOperand::value_type_t as_vector() const;
    unsigned long as_int() const;
    double as_real() const;
    StringTable::Key as_string() const;

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
    
    std::string to_string() const;
};

// For named operands (label, variable, function)
ADD_OPERAND_TYPE(LabelOperand, unsigned)
ADD_OPERAND_TYPE(VariableOperand, std::string_view)
ADD_OPERAND_TYPE(FunctionOperand, unsigned)

using operand_type_t =
    std::variant<ValueOperand, LabelOperand, VariableOperand, FunctionOperand>;

class IrFunction;

class Operand {
   public:
    Operand() : m_used { false }, m_data {} {}
    Operand(operand_type_t data) : m_used { true }, m_data { data } {}

    bool is_value() const;
    bool is_label() const;
    bool is_variable() const;
    bool is_function() const;

    bool is_used() const { return m_used; }

    auto env() const { return m_env; }
    void set_env(const IrFunction* env) { m_env = env; }

    std::string to_string() const;

    ValueOperand as_value() const;
    VariableOperand as_variable() const;
    LabelOperand as_label() const;
    FunctionOperand as_function() const;

   private:
    template <class T>
    bool holds() const {
        return std::holds_alternative<T>(m_data);
    }

    bool m_used;
    operand_type_t m_data;

    const IrFunction* m_env { nullptr };
};

void print_value_operand(value_operand_t);
std::string value_operand_to_string(ValueOperand);
