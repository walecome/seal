#pragma once

#include <vector>

#include "common/PoolEntry.hh"
#include "common/Value.hh"

class ValuePool {
   public:
    ValuePool();

    PoolEntry create_boolean(bool value);
    PoolEntry create_integer(int value);
    PoolEntry create_real(double value);
    PoolEntry create_string(std::string value);
    PoolEntry create_vector(std::vector<PoolEntry> value);

    PoolEntry copy(PoolEntry entry);

   private:
    std::vector<std::unique_ptr<Value>> m_values {};
};
