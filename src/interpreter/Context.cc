#include "interpreter/Context.hh"

#include "Constants.hh"

Context::Context(const ValuePool* constant_pool)
    : m_constant_pool(constant_pool),
      m_dynamic_pool(std::make_unique<ValuePool>(PoolEntry::Type::Dynamic)) {
}

Value& Context::get_value(PoolEntry entry) const {
    switch (entry.type()) {
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
