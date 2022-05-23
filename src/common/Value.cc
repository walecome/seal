#include "common/Value.hh"

#include <fmt/format.h>

#include "Constants.hh"

Value::Value(ValueType type) : m_type(type) {
}

bool Value::is_boolean() const {
    return m_type == ValueType::Boolean;
}

bool Value::is_integer() const {
    return m_type == ValueType::Integer;
}

bool Value::is_real() const {
    return m_type == ValueType::Real;
}

bool Value::is_string() const {
    return m_type == ValueType::String;
}

bool Value::is_vector() const {
    return m_type == ValueType::Vector;
}

Boolean& Value::as_boolean() {
    ASSERT(is_boolean());
    return *reinterpret_cast<Boolean*>(this);
}

Integer& Value::as_integer() {
    ASSERT(is_integer());
    return *reinterpret_cast<Integer*>(this);
}

Real& Value::as_real() {
    ASSERT(is_real());
    return *reinterpret_cast<Real*>(this);
}

String& Value::as_string() {
    ASSERT(is_string());
    return *reinterpret_cast<String*>(this);
}

Vector& Value::as_vector() {
    ASSERT(is_vector());
    return *reinterpret_cast<Vector*>(this);
}

const Boolean& Value::as_boolean() const {
    ASSERT(is_boolean());
    return *reinterpret_cast<const Boolean*>(this);
}

const Integer& Value::as_integer() const {
    ASSERT(is_integer());
    return *reinterpret_cast<const Integer*>(this);
}

const Real& Value::as_real() const {
    ASSERT(is_real());
    return *reinterpret_cast<const Real*>(this);
}

const String& Value::as_string() const {
    ASSERT(is_string());
    return *reinterpret_cast<const String*>(this);
}

const Vector& Value::as_vector() const {
    ASSERT(is_vector());
    return *reinterpret_cast<const Vector*>(this);
}

ValueType Value::type() const {
    return m_type;
}

namespace {

template <class Pred>
bool compare_vector(const Vector&, const Vector&, Pred) {
    // FIXME: It's a bit tricky to compare the elements in a vector since we
    //       need to look up the Value pointed to by the PoolEntry. Sadly,
    // we don't have access to a Context here. So figure out how this is best
    // structured.
    return false;
}

template <class Predicate>
bool compare(const Value& a, const Value& b, Predicate pred) {
    if (a.type() != b.type()) {
        return false;
    }
    if (a.is_boolean()) {
        return pred(a.as_boolean().value(), b.as_boolean().value());
    }
    if (a.is_integer()) {
        return pred(a.as_integer().value(), b.as_integer().value());
    }
    if (a.is_real()) {
        return pred(a.as_real().value(), b.as_real().value());
    }
    if (a.is_string()) {
        return pred(a.as_string().value(), b.as_string().value());
    }
    if (a.is_vector()) {
        return compare_vector(a.as_vector(), b.as_vector(), pred);
    }
    ASSERT_NOT_REACHED();
    return false;
}
}  // namespace

bool Value::operator==(const Value& other) const {
    return compare(*this, other,
                   [](const auto& a, const auto& b) { return a == b; });
}

bool Value::operator!=(const Value& other) const {
    return compare(*this, other,
                   [](const auto& a, const auto& b) { return a != b; });
}

bool Value::operator<(const Value& other) const {
    return compare(*this, other,
                   [](const auto& a, const auto& b) { return a < b; });
}

bool Value::operator>(const Value& other) const {
    return compare(*this, other,
                   [](const auto& a, const auto& b) { return a > b; });
}

bool Value::operator<=(const Value& other) const {
    return compare(*this, other,
                   [](const auto& a, const auto& b) { return a <= b; });
}

bool Value::operator>=(const Value& other) const {
    return compare(*this, other,
                   [](const auto& a, const auto& b) { return a >= b; });
}

Value::operator bool() const {
    if (is_boolean()) {
        return as_boolean().value();
    }
    if (is_integer()) {
        return !!as_integer().value();
    }
    if (is_real()) {
        return !!as_real().value();
    }
    if (is_string()) {
        return !as_string().value().empty();
    }
    if (is_vector()) {
        return !as_vector().value().empty();
    }
    ASSERT_NOT_REACHED();
    return false;
}

Boolean::Boolean(bool value) : Value(ValueType::Boolean), m_value(value) {
}

Boolean::~Boolean() = default;

bool Boolean::value() const {
    return m_value;
}

std::string Boolean::to_string() const {
  return fmt::format("{}", value());
}

Integer::Integer(int value) : Value(ValueType::Integer), m_value(value) {
}

Integer::~Integer() = default;

int Integer::value() const {
    return m_value;
}

std::string Integer::to_string() const {
  return fmt::format("{}", value());
}

Real::Real(double value) : Value(ValueType::Real), m_value(value) {
}

Real::~Real() = default;

double Real::value() const {
    return m_value;
}

std::string Real::to_string() const {
  return fmt::format("{}", value());
}

String::String(std::string value)
    : Value(ValueType::String), m_value(std::move(value)) {
}

String::~String() = default;

size_t String::length() const {
    return m_value.length();
}

std::string_view String::value() const {
    return m_value;
}

std::string String::to_string() const {
  return std::string(value());
}

Vector::Vector(std::vector<PoolEntry> value)
    : Value(ValueType::Vector), m_value(std::move(value)) {
}

Vector::~Vector() = default;

size_t Vector::length() const {
    return m_value.size();
}

PoolEntry Vector::at(size_t index) const {
    return m_value.at(index);
}

void Vector::set(size_t index, PoolEntry entry) {
    m_value.at(index) = entry;
}

void Vector::add(PoolEntry entry) {
    m_value.push_back(entry);
}

const std::vector<PoolEntry>& Vector::value() const {
    return m_value;
}

std::string Vector::to_string() const {
  // FIXME: Only have access to PoolEntry, need to print value.
  return "[Vector::to_string() not implemented]";
}
