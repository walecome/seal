#pragma once

#include <memory>
#include <vector>

#include "common/Value.hh"

#include "Constants.hh"

class ConstantPool {
   public:
    class Entry {
       public:
        std::string to_string() const;

       private:
        explicit Entry(size_t key);
        const size_t key;
        friend class ConstantPool;
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
