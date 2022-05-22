#include "interpreter/Context.hh"

#include "Constants.hh"

Value& Context::get_value(PoolEntry entry) const {
  switch(entry.type()) {
    case PoolEntry::Type::Constant:
      return get_constant_value(entry);
    case PoolEntry::Type::Dynamic:
      return get_dynamic_value(entry);
  }
  ASSERT_NOT_REACHED_MSG("Unexpected PoolEntry::Type");
}


ValuePool& Context::dynamic_pool() {
  return *m_dynamic_pool;
}

const ValuePool& Context::constant_pool() {
  return *m_constant_pool;
}

Value& Context::get_constant_value(PoolEntry entry) const {
  return m_constant_pool->get_entry(entry);
}

Value& Context::get_dynamic_value(PoolEntry entry) const {
  return m_dynamic_pool->get_entry(entry);
}


