#include "common/ConstantPool.hh"

#include <fmt/format.h>

ConstantPool::ConstantPool() = default;

ConstantPool::Entry ConstantPool::add(Value value) {
    size_t key = m_values.size();
    m_values.push_back(std::move(value));
    return Entry(key);
}

Value ConstantPool::get_value(ConstantPool::Entry entry) const {
    if (entry.key >= m_values.size()) {
        ASSERT_NOT_REACHED_MSG("Out of bounds ConstantPool entry");
    }
    return m_values.at(entry.key);
}

void ConstantPool::dump() const {
    for (size_t i = 0; i < m_values.size(); ++i) {
        fmt::print("{}: {}\n", i, m_values.at(i).to_debug_string());
    }
}
