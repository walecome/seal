#pragma once

#include <memory>
#include <vector>

#include "common/Value.hh"

#include "Constants.hh"

class ConstantPool {
   public:
    struct Entry {
        std::string to_string() const;
        explicit Entry(size_t key);
        size_t key;
    };
    explicit ConstantPool();

    ConstantPool(const ConstantPool& other) = delete;
    ConstantPool& operator=(const ConstantPool& other) = delete;

    Entry add(Value value);
    Value get_value(Entry entry) const;

    void dump() const;

   private:
    std::vector<Value> m_values;
};
