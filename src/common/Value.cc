#include "common/Value.hh"

#include <fmt/format.h>

#include "Constants.hh"

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
    if (!a.is_same_type(b)) {
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

struct TypeVisitor {
    template <class T, class U>
    bool operator()(T, U) {
        return false;
    }

    template <class T>
    bool operator()(T) {
        return true;
    }
};

struct Stringifier {
    template <class T>
    std::string operator()(T t) {
        return t.to_string();
    }
};

struct DebugStringifier {
    template <class T>
    std::string operator()(T t) {
        return t.to_debug_string();
    }
};
}  // namespace

std::string None::to_string() const {
    return "None";
}

std::string None::to_debug_string() const {
    return "None";
}

Value::Value() : m_value(None()) {
}

Value::~Value() = default;

Value Value::create_boolean(bool base_value) {
    return Value(Boolean(base_value));
}

Value Value::create_integer(int base_value) {
    return Value(Integer(base_value));
}

Value Value::create_real(double base_value) {
    return Value(Real(base_value));
}

Value Value::create_string(std::string_view base_value) {
    return Value(Object(String(std::string(base_value))));
}

Value Value::copy(Value other) {
    if (other.is_boolean()) {
        return create_boolean(other.as_boolean().value());
    }

    if (other.is_integer()) {
        return create_integer(other.as_integer().value());
    }

    if (other.is_real()) {
        return create_real(other.as_real().value());
    }

    if (other.is_string()) {
        return create_string(other.as_string().value());
    }

    if (other.is_vector()) {
        // FIXME: Copy Vector
        ASSERT_NOT_REACHED();
    }

    ASSERT_NOT_REACHED();
}

Value::Value(value_t&& value) : m_value(std::move(value)) {
}

std::string Value::stringify() const {
    return std::visit(Stringifier {}, m_value);
}

std::string Value::to_debug_string() const {
    return std::visit(DebugStringifier {}, m_value);
}

bool Value::is_boolean() const {
    return std::holds_alternative<Boolean>(m_value);
}

bool Value::is_integer() const {
    return std::holds_alternative<Integer>(m_value);
}

bool Value::is_real() const {
    return std::holds_alternative<Real>(m_value);
}

bool Value::is_object() const {
    return std::holds_alternative<Object>(m_value);
}

bool Value::is_string() const {
    return is_object() && as_object().is_string();
}

bool Value::is_vector() const {
    return is_object() && as_object().is_vector();
}

Boolean Value::as_boolean() const {
    ASSERT(is_boolean());
    return std::get<Boolean>(m_value);
}

Integer Value::as_integer() const {
    ASSERT(is_integer());
    return std::get<Integer>(m_value);
}

Real Value::as_real() const {
    ASSERT(is_real());
    return std::get<Real>(m_value);
}

Object& Value::as_object() {
    ASSERT(is_object());
    return std::get<Object>(m_value);
}

const Object& Value::as_object() const {
    ASSERT(is_object());
    return std::get<Object>(m_value);
}

String& Value::as_string() {
    ASSERT(is_string());
    return as_object().as_string();
}

Vector& Value::as_vector() {
    ASSERT(is_vector());
    return as_object().as_vector();
}

const String& Value::as_string() const {
    ASSERT(is_string());
    return as_object().as_string();
}

const Vector& Value::as_vector() const {
    ASSERT(is_vector());
    return as_object().as_vector();
}

bool Value::is_same_type(Value other) const {
    return std::visit(TypeVisitor {}, m_value, other.m_value);
}

bool Value::is_truthy() const {
    if (is_boolean()) {
        return as_boolean().value();
    }

    if (is_integer()) {
        return as_integer().value() != 0;
    }

    if (is_real()) {
        double tmp = as_real().value();
        return tmp < 0 || tmp > 0;
    }

    if (is_string()) {
        return !as_string().value().empty();
    }

    if (is_vector()) {
        return !as_vector().value().empty();
    }

    ASSERT_NOT_REACHED();
}

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

Boolean::Boolean(bool value) : m_value(value) {
}

Boolean::~Boolean() = default;

bool Boolean::value() const {
    return m_value;
}

std::string Boolean::to_string() const {
    return fmt::format("{}", value());
}

std::string Boolean::to_debug_string() const {
    return fmt::format("{} (Boolean)", value());
}

Integer::Integer(int value) : m_value(value) {
}

Integer::~Integer() = default;

int Integer::value() const {
    return m_value;
}

std::string Integer::to_string() const {
    return fmt::format("{}", value());
}

std::string Integer::to_debug_string() const {
    return fmt::format("{} (Integer)", value());
}

Real::Real(double value) : m_value(value) {
}

Real::~Real() = default;

double Real::value() const {
    return m_value;
}

std::string Real::to_string() const {
    return fmt::format("{}", value());
}

std::string Real::to_debug_string() const {
    return fmt::format("{} (Real)", value());
}

Object::Object() : m_value(None()) {
}

Object::Object(object_value_t&& value) : m_value(value) {
}

Object::~Object() = default;

std::string Object::to_string() const {
    return std::visit(Stringifier {}, m_value);
}

std::string Object::to_debug_string() const {
    return std::visit(DebugStringifier {}, m_value);
}

bool Object::is_string() const {
    return std::holds_alternative<String>(m_value);
}

bool Object::is_vector() const {
    return std::holds_alternative<Vector>(m_value);
}

String& Object::as_string() {
    return std::get<String>(m_value);
}

Vector& Object::as_vector() {
    return std::get<Vector>(m_value);
}

const String& Object::as_string() const {
    return std::get<String>(m_value);
}

const Vector& Object::as_vector() const {
    return std::get<Vector>(m_value);
}

String::String(std::string value) : m_value(std::move(value)) {
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

std::string String::to_debug_string() const {
    return fmt::format("\"{}\" (String)", value());
}

Vector::Vector(std::vector<Value>&& value) : m_value(value) {
}

Vector::~Vector() = default;

size_t Vector::length() const {
    return m_value.size();
}

Value Vector::at(size_t index) const {
    return m_value.at(index);
}

void Vector::set(size_t index, Value value) {
    m_value.at(index) = value;
}

void Vector::add(Value value) {
    m_value.push_back(value);
}

const std::vector<Value>& Vector::value() const {
    return m_value;
}

std::string Vector::to_string() const {
    std::vector<std::string> stringified_values;
    for (Value value : value()) {
        stringified_values.push_back(value.stringify());
    }
    return fmt::format("[{}]", fmt::join(stringified_values, ", "));
}

std::string Vector::to_debug_string() const {
    std::vector<std::string> stringified_values;
    for (Value value : value()) {
        stringified_values.push_back(value.to_debug_string());
    }
    return fmt::format("[{}]", fmt::join(stringified_values, ", "));
}
