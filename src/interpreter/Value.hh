#pragma once

#include <memory>
#include <variant>

#include "utility/StringTable.hh"

class Boolean {
   public:
    explicit Boolean(bool value);
};

class Integer {
   public:
    explicit Integer(int value);
};

class Real {
   public:
    explicit Real(double value);
};

class String {
   public:
    explicit String(std::string runtime_string);
    explicit String(StringTable::Entry compiletime_string);

    const std::string& resolve(StringTable* string_table);

   private:
    std::variant<std::string, StringTable::Entry> m_data;
};

class Value;

class Vector {
   public:
    explicit Vector(std::vector<Value> values);

   private:
    std::vector<Value> m_values;
};

class Value {
   public:
    Value() = delete;
    virtual ~Value() = default;

    bool is_vector() const;
    bool is_string() const;
    bool is_integer() const;
    bool is_real() const;
    bool is_boolean() const;

    std::variant<Integer, Real, String, Vector, Boolean>& data() const;
};
