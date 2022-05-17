#pragma once

#include <cstddef>

struct PoolEntry {
    static PoolEntry create_constant(size_t key);
    static PoolEntry create_dynamic(size_t key);

    const size_t key;
    const bool constant;

   private:
    PoolEntry(size_t key, bool constant);
};
