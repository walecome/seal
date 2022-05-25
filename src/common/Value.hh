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

    virtual std::string to_string() const = 0;
    virtual std::string to_debug_string() const = 0;

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

    virtual bool is_mutable() const;

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

class Boolean : public Value {
   public:
    explicit Boolean(bool value);
    ~Boolean();

    bool value() const;

    std::string to_string() const override;
    std::string to_debug_string() const override;

   private:
    const bool m_value;
};

class Integer : public Value {
   public:
    explicit Integer(int value);
    ~Integer();

    int value() const;

    std::string to_string() const override;
    std::string to_debug_string() const override;

   private:
    const int m_value;
};

class Real : public Value {
   public:
    explicit Real(double value);

    ~Real();

    double value() const;

    std::string to_string() const override;
    std::string to_debug_string() const override;

   private:
    const double m_value;
};

class String : public Value {
   public:
    explicit String(std::string value);
    ~String();

    size_t length() const;

    std::string_view value() const;

    std::string to_string() const override;
    std::string to_debug_string() const override;

   private:
    const std::string m_value;
};

class Vector : public Value {
   public:
    explicit Vector(std::vector<PoolEntry> values);
    ~Vector();

    const std::vector<PoolEntry>& value() const;

    size_t length() const;
    PoolEntry at(size_t index) const;
    void set(size_t index, PoolEntry entry);
    void add(PoolEntry entry);

    bool is_mutable() const override;

    std::string to_string() const override;
    std::string to_debug_string() const override;

   private:
      std::vector<PoolEntry> m_value;
};
