#pragma once

#include <cstddef>

struct PoolEntry {
    PoolEntry();

    static PoolEntry create_constant(size_t key);
    static PoolEntry create_dynamic(size_t key);

    // TODO: Maybe move ValuePool::Type here, and switch that that from bool.
    const size_t key;
    const bool constant;

   private:
    PoolEntry(size_t key, bool constant);
};
