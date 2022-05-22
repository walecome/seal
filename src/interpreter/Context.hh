#pragma once

#include <memory>

#include "common/ValuePool.hh"

class Value;

class Context {
  public:
    Value& get_value(PoolEntry entry);
    Value& get_value(PoolEntry entry) const;
    ValuePool& dynamic_pool();
    const ValuePool& constant_pool();

  private:
    const std::unique_ptr<const ValuePool> m_constant_pool;
    const std::unique_ptr<ValuePool> m_dynamic_pool;
};
