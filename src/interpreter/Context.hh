#pragma once

#include <memory>

#include "common/ValuePool.hh"
#include "common/ValueResolver.hh"

class Value;

class Context : public ValueResolver {
   public:
    explicit Context(const ValuePool* constant_pool);
    Value& get_value(PoolEntry entry) const override;
    ValuePool& dynamic_pool();
    const ValuePool& constant_pool();

   private:
    Value& get_constant_value(PoolEntry entry) const;
    Value& get_dynamic_value(PoolEntry entry) const;

    const ValuePool* m_constant_pool;
    const std::unique_ptr<ValuePool> m_dynamic_pool;
};
