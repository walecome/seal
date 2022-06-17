#include "common/BuiltinFunctionAddress.hh"

BuiltinFunctionAddress::BuiltinFunctionAddress() = default;

BuiltinFunctionAddress::BuiltinFunctionAddress(size_t id) : m_id(id) {
}

size_t BuiltinFunctionAddress::id() const {
  return m_id;
}
