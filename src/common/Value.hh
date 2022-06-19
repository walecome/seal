#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "Constants.hh"

class Value;

struct None {
    std::string to_string() const;
    std::string to_debug_string() const;
};

class Boolean {
   public:
    explicit Boolean(bool value);
    ~Boolean();

    bool value() const;

    std::string to_string() const;
    std::string to_debug_string() const;

   private:
    bool m_value;
};

class Integer {
   public:
    explicit Integer(int value);
    ~Integer();

    int value() const;

    std::string to_string() const;
    std::string to_debug_string() const;

   private:
    int m_value;
};

class Real {
   public:
    explicit Real(double value);
    ~Real();

    double value() const;

    std::string to_string() const;
    std::string to_debug_string() const;

   private:
    double m_value;
};

class String {
   public:
    String(std::string value);
    ~String();

    size_t length() const;

    std::string_view value() const;

    std::string to_string() const;
    std::string to_debug_string() const;

   private:
    std::string m_value;
};

class Vector {
   public:
    Vector();
    ~Vector();

    const std::vector<Value>* value() const;

    size_t length() const;
    Value at(size_t index) const;
    void set(size_t index, Value entry);
    void add(Value entry);

    std::string to_string() const;
    std::string to_debug_string() const;

   private:
    std::shared_ptr<std::vector<Value>> m_value;
};

class Object {
   public:
    Object();
    ~Object();

    std::string to_string() const;
    std::string to_debug_string() const;

    String& as_string();

    bool is_string() const;
    bool is_vector() const;

    Vector& as_vector();

    const String& as_string() const;
    const Vector& as_vector() const;

   private:
    using object_value_t = std::variant<None, String, Vector>;
    Object(object_value_t&& value);

    object_value_t m_value;
    friend class Value;
};

class Value final {
   public:
    Value();
    virtual ~Value();

    static Value create_boolean(bool base_value);
    static Value create_integer(int base_value);
    static Value create_real(double base_value);
    static Value create_string(std::string_view base_value);
    static Value create_vector();
    static Value copy(Value other);

    std::string stringify() const;
    std::string to_debug_string() const;

    bool is_boolean() const;
    bool is_integer() const;
    bool is_real() const;
    bool is_object() const;
    bool is_string() const;
    bool is_vector() const;
    bool is_none() const;

    Boolean as_boolean() const;
    Integer as_integer() const;
    Real as_real() const;

    String& as_string();
    Vector& as_vector();

    const String& as_string() const;
    const Vector& as_vector() const;

    bool is_same_type(Value other) const;

    bool is_truthy() const;

    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const;
    bool operator<(const Value& other) const;
    bool operator>(const Value& other) const;
    bool operator<=(const Value& other) const;
    bool operator>=(const Value& other) const;

   private:
    using value_t = std::variant<None, Boolean, Integer, Real, Object>;

    Value(value_t&& value);

    Object& as_object();
    const Object& as_object() const;

    value_t m_value;
};
