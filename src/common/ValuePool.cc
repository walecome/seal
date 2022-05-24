#include "common/ValuePool.hh"

#include <fmt/format.h>

namespace {
template <class SealValueType, class CppValueType>
PoolEntry add_new_entry(std::vector<std::unique_ptr<Value>>& values,
                        PoolEntry::Type type, CppValueType data) {
    size_t key = values.size();
    PoolEntry entry = type == PoolEntry::Type::Constant
                          ? PoolEntry::create_constant(key)
                          : PoolEntry::create_dynamic(key);
    values.push_back(std::make_unique<SealValueType>(data));
    return entry;
}
}  // namespace

ValuePool::ValuePool(PoolEntry::Type type) : m_type(type) {
}

PoolEntry ValuePool::create_boolean(bool value) {
    return add_new_entry<Boolean>(m_values, m_type, value);
}

PoolEntry ValuePool::create_integer(int value) {
    return add_new_entry<Integer>(m_values, m_type, value);
}

PoolEntry ValuePool::create_real(double value) {
    return add_new_entry<Real>(m_values, m_type, value);
}

PoolEntry ValuePool::create_string(std::string_view value) {
    return add_new_entry<String>(m_values, m_type, std::string(value));
}

PoolEntry ValuePool::create_string(std::string value) {
    return add_new_entry<String>(m_values, m_type, std::move(value));
}

PoolEntry ValuePool::create_vector(std::vector<PoolEntry> value) {
    return add_new_entry<Vector>(m_values, m_type, std::move(value));
}

PoolEntry ValuePool::copy(PoolEntry entry) {
    const Value& current = *m_values[entry.key()];

    if (current.is_boolean()) {
        return create_boolean(current.as_boolean().value());
    }
    if (current.is_integer()) {
        return create_integer(current.as_integer().value());
    }
    if (current.is_real()) {
        return create_real(current.as_real().value());
    }
    if (current.is_string()) {
        return create_string(current.as_string().value());
    }
    if (current.is_vector()) {
        return create_vector(current.as_vector().value());
    }

    ASSERT_NOT_REACHED();
}

Value& ValuePool::get_entry(PoolEntry entry) const {
    if (entry.key() >= m_values.size()) {
        ASSERT_NOT_REACHED_MSG("Out of bounds ValuePool entry");
    }
    return *m_values.at(entry.key());
}

void ValuePool::dump() const {
  for (size_t i = 0; i < m_values.size(); ++i) {
    fmt::print("{}: {}\n", i, m_values.at(i)->to_debug_string());
  }
}
