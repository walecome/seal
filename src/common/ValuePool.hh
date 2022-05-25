#pragma once

#include <memory>
#include <vector>

#include "common/PoolEntry.hh"
#include "common/Value.hh"

#include "Constants.hh"

class ValuePool {
   public:
    explicit ValuePool(PoolEntry::Type type);

    ValuePool(const ValuePool& other) = delete;
    ValuePool& operator=(const ValuePool& other) = delete;

    PoolEntry create_boolean(bool value);
    PoolEntry create_integer(int value);
    PoolEntry create_real(double value);
    PoolEntry create_string(std::string value);
    PoolEntry create_string(std::string_view value);
    PoolEntry create_vector(std::vector<PoolEntry> value);

    Value& get_entry(PoolEntry entry) const;

    void dump() const;

   private:
    PoolEntry::Type m_type;
    std::vector<std::unique_ptr<Value>> m_values;
};
