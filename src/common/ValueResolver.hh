#pragma once

#include "common/PoolEntry.hh"

class Value;

class ValueResolver {
   public:
    virtual ~ValueResolver() = default;
    virtual Value& get_value(PoolEntry entry) const = 0;
};
