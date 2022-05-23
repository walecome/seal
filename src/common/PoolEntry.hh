#pragma once

#include <cstddef>

class PoolEntry {
   public:
    enum class Type {
        Constant,
        Dynamic,
    };

    PoolEntry();

    static PoolEntry create_constant(size_t key);
    static PoolEntry create_dynamic(size_t key);

    PoolEntry(const PoolEntry& other);
    PoolEntry& operator=(const PoolEntry& other);

    bool operator==(const PoolEntry& other) const;

    size_t key() const;
    Type type() const;

   private:
    PoolEntry(size_t key, Type type);

    size_t m_key;
    Type m_type;
};
