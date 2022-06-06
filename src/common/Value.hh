#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "common/PoolEntry.hh"

class Value;
class ValueResolver;

class Boolean {
   public:
    ~Boolean();

    bool value() const;

    std::string to_string(const ValueResolver& resolver) const;
    std::string to_debug_string(const ValueResolver& resolver) const;

   private:
    explicit Boolean(bool value);

    const bool m_value;
    friend class Value;
};

class Integer {
   public:
    ~Integer();

    int value() const;

    std::string to_string(const ValueResolver& resolver) const;
    std::string to_debug_string(const ValueResolver& resolver) const;

   private:
    explicit Integer(int value);

    const int m_value;

    friend class Value;
};

class Real {
   public:
    ~Real();

    double value() const;

    std::string to_string(const ValueResolver& resolver) const;
    std::string to_debug_string(const ValueResolver& resolver) const;

   private:
    explicit Real(double value);

    const double m_value;

    friend class Value;
};

class String {
   public:
    size_t length() const;

    std::string_view value() const;

    std::string to_string(const ValueResolver& resolver) const;
    std::string to_debug_string(const ValueResolver& resolver) const;

   private:
};

class Vector {
   public:
    ~Vector();

    const std::vector<Value>& value() const;

    size_t length() const;
    Value at(size_t index) const;
    void set(size_t index, Value entry);
    void add(Value entry);

    std::string to_string(const ValueResolver& resolver) const;
    std::string to_debug_string(const ValueResolver& resolver) const;

   private:
};

class Value final {
   public:
    Value();
    virtual ~Value();

    static Value create_boolean(bool base_value);
    static Value create_integer(int base_value);
    static Value create_real(double base_value);
    static Value copy(Value other);

    std::string to_string() const;
    std::string to_debug_string() const;

    template <class T>
    bool is() const {
        return std::holds_alternative<T>(m_base_value);
    }

    template <class T>
    T as() const {
        ASSERT(is<T>());
        return std::get<T>(m_base_value);
    }

    bool is_same_type(Value other) const;

    bool is_truthy() const;

    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const;
    bool operator<(const Value& other) const;
    bool operator>(const Value& other) const;
    bool operator<=(const Value& other) const;
    bool operator>=(const Value& other) const;

   protected:
   private:
    std::variant<Boolean, Integer, Real, String, Vector> m_base_value;
};
