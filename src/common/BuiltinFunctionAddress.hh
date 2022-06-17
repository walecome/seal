#pragma once

#include <cstddef>

class BuiltinFunctionAddress {
   public:
     BuiltinFunctionAddress();
     BuiltinFunctionAddress(size_t id);
    size_t id() const;
   private:
    size_t m_id;
};
