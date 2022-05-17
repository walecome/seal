#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "common/PoolEntry.hh"

enum class ValueType { Boolean, Integer, Real, String, Vector };

class Value {
   public:
    Value() = delete;
    virtual ~Value() = default;

    virtual std::string to_string() = 0;

   protected:
    Value(ValueType type);

   private:
    ValueType m_type;
};

class Boolean : public Value {
   public:
    explicit Boolean(bool value);

    ~Boolean() override;

    bool value() const;
    bool operator==(const Boolean& other) const;

   private:
    const bool m_value;
};

class Integer : public Value {
   public:
    explicit Integer(int value);

    ~Integer() override;

    bool operator==(const Integer& other) const;

    int value() const;

   private:
    const int m_value;
};

class Real : public Value {
   public:
    explicit Real(double value);

    ~Real() override;

    bool operator==(const Real& other) const;

    double value() const;

   private:
    const double m_value;
};

class String : public Value {
   public:
    explicit String(std::string runtime_string);

    ~String() override;

    bool operator==(const String& other) const;

   private:
    const std::string m_value;
};

class Vector : public Value {
   public:
    explicit Vector(std::vector<PoolEntry> values);

    bool operator==(const Vector& other) const;

   private:
};
