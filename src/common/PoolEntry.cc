#include "common/PoolEntry.hh"

#include <utility>

PoolEntry::PoolEntry() : m_key(-1), m_type(PoolEntry::Type::Constant) {
}

PoolEntry::PoolEntry(const PoolEntry& other)
    : m_key(other.key()), m_type(other.type()) {
}

PoolEntry& PoolEntry::operator=(const PoolEntry& other) {
    m_key = other.key();
    m_type = other.type();
    return *this;
}

size_t PoolEntry::key() const {
    return m_key;
}

PoolEntry::Type PoolEntry::type() const {
    return m_type;
}
