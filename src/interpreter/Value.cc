#include "interpreter/Value.hh"

#include "Constants.hh"

#include <utility>

// BOOLEAN

Boolean::Boolean(bool value) : m_value(value) {
}

bool Boolean::operator==(const Boolean& other) const {
    return m_value == other.m_value;
}

bool Boolean::value() const {
    return m_value;
}

// INTEGER

Integer::Integer(int value) : m_value(value) {
}

bool Integer::operator==(const Integer& other) const {
    return m_value == other.m_value;
}

int Integer::value() const {
    return m_value;
}

// REAL

Real::Real(double value) : m_value(value) {
}

bool Real::operator==(const Real& other) const {
    return m_value == other.m_value;
}

double Real::value() const {
    return m_value;
}

// STRING

String::String(std::string runtime_string) : m_data(std::move(runtime_string)) {
}
String::String(StringTable::Key compiletime_string)
    : m_data(std::move(compiletime_string)) {
}

bool String::operator==(const String& other) const {
    // FIXME: We don't have access to the string table here, which makes string
    // compare difficult... Currently thinking that we shouldn't do compare of
    // the underlaying value here, as that is handled in the interpreter.
    ASSERT_NOT_REACHED_MSG("String::operator==() not implemented");
}

std::string_view String::resolve(const StringTable& string_table) const {
    if (std::holds_alternative<std::string>(m_data)) {
        return std::get<std::string>(m_data);
    }
    if (std::holds_alternative<StringTable::Key>(m_data)) {
        auto entry = std::get<StringTable::Key>(m_data);
        return string_table.get_at(entry);
    }
    ASSERT_NOT_REACHED();
}

// VECTOR

Vector::Vector(std::vector<Value> values)
// : m_values(std::move(values))
{
}

bool Vector::operator==(const Vector& other) const {
    ASSERT_NOT_REACHED_MSG("Vector::operator==() not implemented");
}

const std::vector<Value>& Vector::values() const {
    // FIXME
    // return m_values;
    return std::vector<Value> {};
}

std::vector<Value>& Vector::mutable_values() {
    // FIXME
    auto tmp = std::vector<Value> {};
    return tmp;
    // return m_values;
}

// VALUE

Value::Value() : m_data(nullptr) {
}

Value::Value(Value&& other) = default;

Value& Value::operator=(Value&& other) {
    m_data = std::move(other.m_data);
    return *this;
}

bool Value::operator==(const Value& other) const {
    if (!is_same_type(other)) {
        return false;
    }
    return data() == other.data();
}

Value Value::copy() const {
    ASSERT_NOT_REACHED_MSG("Value::copy() not implemented");
}

struct TypeComparator {
    template <class T>
    bool operator()(T, T) {
        return true;
    }

    template <class T, class U>
    bool operator()(T, U) {
        return false;
    }
};

bool Value::is_same_type(const Value& other) const {
    return std::visit(TypeComparator {}, data(), other.data());
}

bool Value::is_vector() const {
    return std::holds_alternative<Vector>(data());
}

bool Value::is_string() const {
    return std::holds_alternative<String>(data());
}

bool Value::is_integer() const {
    return std::holds_alternative<Integer>(data());
}

bool Value::is_real() const {
    return std::holds_alternative<Real>(data());
}

bool Value::is_boolean() const {
    return std::holds_alternative<Boolean>(data());
}

Vector Value::as_vector() const {
    return std::get<Vector>(data());
}

String Value::as_string() const {
    return std::get<String>(data());
}

Integer Value::as_integer() const {
    return std::get<Integer>(data());
}

Real Value::as_real() const {
    return std::get<Real>(data());
}

Boolean Value::as_boolean() const {
    return std::get<Boolean>(data());
}

const Value::value_type_t& Value::data() const {
    ASSERT(!!m_data);

    return *m_data;
}
