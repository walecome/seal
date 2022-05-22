#pragma once

#include <memory>
#include <string>
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
    Value() = default;
    virtual ~Value() = default;

    virtual std::string to_string() = 0;

    bool is_string() const;
    bool is_vector() const;
    bool is_integer() const;
    bool is_boolean() const;
    bool is_real() const;

    String& as_string();
    Boolean& as_boolean();
    Vector& as_vector();
    Integer as_integer();
    Real as_real();

    const String& as_string() const;
    const Boolean& as_boolean() const;
    const Vector& as_vector() const;
    const Integer as_integer() const;
    const Real as_real() const;

    ValueType type() const;
    ValueType type();

    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const;
    bool operator<(const Value& other) const;
    bool operator>(const Value& other) const;
    bool operator<=(const Value& other) const;
    bool operator>=(const Value& other) const;
    bool operator&&(const Value& other) const;
    bool operator||(const Value& other) const;

   protected:
    Value(ValueType type);

   private:
    ValueType m_type;
};

class Boolean : Value {
   public:
    explicit Boolean(bool value);

    ~Boolean() override;

    bool value() const;
    bool operator==(const Boolean& other) const;

   private:
    const bool m_value;
};

class Integer : Value {
   public:
    explicit Integer(int value);

    ~Integer() override;

    bool operator==(const Integer& other) const;

    int value() const;

   private:
    const int m_value;
};

class Real : Value {
   public:
    explicit Real(double value);

    ~Real() override;

    bool operator==(const Real& other) const;

    double value() const;

   private:
    const double m_value;
};

class String : Value {
   public:
    explicit String(std::string runtime_string);

    ~String() override;

    bool operator==(const String& other) const;

    size_t length() const;

    std::string_view value() const;

   private:
    const std::string m_value;
};

class Vector : Value {
   public:
    explicit Vector(std::vector<PoolEntry> values);

    bool operator==(const Vector& other) const;

    size_t length() const;
    PoolEntry at(size_t index) const;
    void set(size_t index, PoolEntry entry);
    void add(PoolEntry entry);

   private:
};
