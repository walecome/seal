#pragma once

#include <memory>
#include <string_view>
#include <variant>

#include "utility/StringTable.hh"

class Boolean {
   public:
    explicit Boolean(bool value);

    bool value() const;

    bool operator==(const Boolean& other) const;

   private:
    const bool m_value;
};

class Integer {
   public:
    explicit Integer(int value);

    bool operator==(const Integer& other) const;

    int value() const;

   private:
    const int m_value;
};

class Real {
   public:
    explicit Real(double value);

    bool operator==(const Real& other) const;

    double value() const;

   private:
    const double m_value;
};

class String {
   public:
    explicit String(std::string runtime_string);
    explicit String(StringTable::Key compiletime_string);

    bool operator==(const String& other) const;

    std::string_view resolve(const StringTable& string_table) const;

   private:
    std::variant<std::string, StringTable::Key> m_data;
};

class Value;

class Vector {
   public:
    Vector();
    explicit Vector(std::vector<Value> values);

    bool operator==(const Vector& other) const;

    const std::vector<Value>& values() const;
    std::vector<Value>& mutable_values();

   private:
    // std::vector<Value> m_values;
};

class Value {
  private:
    using value_type_t = std::variant<Integer, Real, String, Vector, Boolean>;

   public:
    Value();

    Value(const Value& other) = delete;
    Value& operator=(const Value& other) = delete;

    Value(Value&& other);
    Value& operator=(Value&& other);

    bool operator==(const Value& other) const;

    Value copy() const;

    bool is_same_type(const Value& other) const;

    bool is_vector() const;
    bool is_string() const;
    bool is_integer() const;
    bool is_real() const;
    bool is_boolean() const;

    Vector& as_vector() const;
    String& as_string() const;
    Integer& as_integer() const;
    Real& as_real() const;
    Boolean& as_boolean() const;

    const value_type_t& data() const;

    std::string to_string() const;

   private:
    value_type_t& mutable_data() const;

    std::unique_ptr<value_type_t> m_data;
};
