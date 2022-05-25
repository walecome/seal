#include "common/PoolEntry.hh"

#include <utility>

#include "Constants.hh"

PoolEntry::PoolEntry() : m_key(std::nullopt), m_type(std::nullopt) {
}

PoolEntry::PoolEntry(size_t key, Type type) : m_key(key), m_type(type) {
}

PoolEntry::PoolEntry(const PoolEntry& other)
    : m_key(other.m_key), m_type(other.m_type) {
}

PoolEntry& PoolEntry::operator=(const PoolEntry& other) {
    m_key = other.m_key;
    m_type = other.m_type;
    return *this;
}

PoolEntry PoolEntry::create_constant(size_t key) {
    return PoolEntry(key, Type::Constant);
}

PoolEntry PoolEntry::create_dynamic(size_t key) {
    return PoolEntry(key, Type::Dynamic);
}

size_t PoolEntry::key() const {
    ASSERT(m_key.has_value());
    return m_key.value();
}

PoolEntry::Type PoolEntry::type() const {
    ASSERT(m_type.has_value());
    return m_type.value();
}

bool PoolEntry::has_value() const {
  return m_type.has_value() && m_key.has_value();
}
