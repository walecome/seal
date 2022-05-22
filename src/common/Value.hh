#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "common/PoolEntry.hh"

enum class ValueType { Boolean, Integer, Real, String, Vector };

class String;
class Boolean;
class Vector;
class Integer;
class Real;

class Value {
   public:
    Value() = delete;
    virtual ~Value() = default;

    virtual std::string to_string() = 0;

    bool is_boolean() const;
    bool is_integer() const;
    bool is_real() const;
    bool is_string() const;
    bool is_vector() const;

    Boolean& as_boolean();
    Integer& as_integer();
    Real& as_real();
    String& as_string();
    Vector& as_vector();

    const Boolean& as_boolean() const;
    const Integer& as_integer() const;
    const Real& as_real() const;
    const String& as_string() const;
    const Vector& as_vector() const;

    ValueType type() const;

    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const;
    bool operator<(const Value& other) const;
    bool operator>(const Value& other) const;
    bool operator<=(const Value& other) const;
    bool operator>=(const Value& other) const;

    explicit operator bool() const;

   protected:
    Value(ValueType type);

   private:
    ValueType m_type;
};

class Boolean : Value {
   public:
    explicit Boolean(bool value);

    bool value() const;

   private:
    const bool m_value;
};

class Integer : Value {
   public:
    explicit Integer(int value);

    int value() const;

   private:
    const int m_value;
};

class Real : Value {
   public:
    explicit Real(double value);

    ~Real() override;

    double value() const;

   private:
    const double m_value;
};

class String : Value {
   public:
    explicit String(std::string runtime_string);

    size_t length() const;

    std::string_view value() const;

   private:
    const std::string m_value;
};

class Vector : Value {
   public:
    explicit Vector(std::vector<PoolEntry> values);

    const std::vector<PoolEntry>& value() const;

    size_t length() const;
    PoolEntry at(size_t index) const;
    void set(size_t index, PoolEntry entry);
    void add(PoolEntry entry);

   private:
      std::vector<PoolEntry> m_value;
};
