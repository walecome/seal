#pragma once

#include <memory>
#include <variant>

#include "utility/StringTable.hh"

class Boolean {
   public:
    explicit Boolean(bool value);

    bool value() const;
};

class Integer {
   public:
    explicit Integer(int value);

    int value() const;
};

class Real {
   public:
    explicit Real(double value);

    double value() const;
};

class String {
   public:
    explicit String(std::string runtime_string);
    explicit String(StringTable::Entry compiletime_string);

    // TODO: Return a std::string_view. Data is either owned by the String class or the string table.
    const std::string& resolve(const StringTable* string_table) const;

   private:
    std::variant<std::string, StringTable::Entry> m_data;
};

class Value;

class Vector {
   public:
    explicit Vector(std::vector<Value> values);

    const std::vector<Value>& values() const;
    std::vector<Value>& mutable_values();

   private:
    std::vector<Value> m_values;
};

class Value {
   public:
    Value() = default;
    virtual ~Value() = default;

    bool is_vector() const;
    bool is_string() const;
    bool is_integer() const;
    bool is_real() const;
    bool is_boolean() const;

    Vector as_vector() const;
    String as_string() const;
    Integer as_integer() const;
    Real as_real() const;
    Boolean as_boolean() const;

    std::variant<Integer, Real, String, Vector, Boolean>& data() const;
};
