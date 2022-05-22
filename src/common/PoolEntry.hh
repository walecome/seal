#pragma once

#include <cstddef>

class PoolEntry {
   public:
    enum class Type {
        Constant,
        Dynamic,
    };

    PoolEntry();

    PoolEntry(const PoolEntry& other);
    PoolEntry& operator=(const PoolEntry& other);

    bool operator==(const PoolEntry& other) const;

    static PoolEntry create_constant(size_t key);
    static PoolEntry create_dynamic(size_t key);

    size_t key() const;
    Type type() const;

   private:
    PoolEntry(size_t key, bool constant);

    size_t m_key;
    Type m_type;
};
