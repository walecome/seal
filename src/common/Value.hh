#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "Constants.hh"

class Value;

class Boolean {
   public:
    ~Boolean();

    bool value() const;

    std::string to_string() const;
    std::string to_debug_string() const;

   private:
    explicit Boolean(bool value);

    const bool m_value;
    friend class Value;
};

class Integer {
   public:
    ~Integer();

    int value() const;

    std::string to_string() const;
    std::string to_debug_string() const;

   private:
    explicit Integer(int value);

    const int m_value;

    friend class Value;
};

class Real {
   public:
    ~Real();

    double value() const;

    std::string to_string() const;
    std::string to_debug_string() const;

   private:
    explicit Real(double value);

    const double m_value;

    friend class Value;
};

class String {
   public:
    size_t length() const;

    std::string_view value() const;

    std::string to_string() const;
    std::string to_debug_string() const;

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

    std::string to_string() const;
    std::string to_debug_string() const;

   private:
};

class Object {
   public:
    Object();
    ~Object();

    bool is_string() const;
    bool is_vector() const;

    String as_string() const;
    Vector as_vector() const;
};

class Value final {
   public:
    Value();
    virtual ~Value();

    static Value create_boolean(bool base_value);
    static Value create_integer(int base_value);
    static Value create_real(double base_value);
    static Value create_string(std::string_view base_value);
    static Value copy(Value other);

    std::string stringify() const;
    std::string to_debug_string() const;

    bool is_boolean() const;
    bool is_integer() const;
    bool is_real() const;
    bool is_object() const;
    bool is_string() const;
    bool is_vector() const;

    Boolean as_boolean() const;
    Integer as_integer() const;
    Real as_real() const;

    String& as_string() const;
    Vector& as_vector() const;

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
};
