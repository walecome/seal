#pragma once

#include <cstddef>

class PoolEntry {
  public:
    PoolEntry();

    PoolEntry(PoolEntry& other);
    PoolEntry(const PoolEntry& other);

    PoolEntry& operator=(PoolEntry& other);
    PoolEntry& operator=(const PoolEntry& other);

    static PoolEntry create_constant(size_t key);
    static PoolEntry create_dynamic(size_t key);

    size_t key() const;

   private:
    PoolEntry(size_t key, bool constant);

    // TODO: Maybe move ValuePool::Type here, and switch that that from bool.
    const size_t m_key;
    const bool m_constant;

};
