#pragma once

#include <cstddef>
#include <optional>

class PoolEntry {
   public:
    enum class Type {
        Constant,
        Dynamic,
    };

    PoolEntry();

    PoolEntry(const PoolEntry& other);
    PoolEntry& operator=(const PoolEntry& other);

    static PoolEntry create_constant(size_t key);
    static PoolEntry create_dynamic(size_t key);

    bool operator==(const PoolEntry& other) const;

    size_t key() const;
    Type type() const;

    bool has_value() const;

   private:
    PoolEntry(size_t key, Type type);

    std::optional<size_t> m_key;
    std::optional<Type> m_type;
};
