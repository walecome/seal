#pragma once

#include <vector>

#include "common/PoolEntry.hh"
#include "common/Value.hh"

#include "Constants.hh"

class ValuePool {
   public:
    enum class Type {
        Constant,
        Dynamic,
    };

    ValuePool(const ValuePool& other);
    ValuePool& operator=(const ValuePool& other);

    explicit ValuePool(Type type);

    PoolEntry create_boolean(bool value);
    PoolEntry create_integer(int value);
    PoolEntry create_real(double value);
    PoolEntry create_string(std::string value);
    PoolEntry create_string(std::string_view value);
    PoolEntry create_vector(std::vector<PoolEntry> value);

    PoolEntry copy(PoolEntry entry);

   private:
    Type m_type;
    std::vector<std::unique_ptr<int>> m_values;
};
