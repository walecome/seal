#include <algorithm>

#include <utility/StringTable.hh>
#include "Constants.hh"

StringTable::Entry StringTable::add(std::string&& s) {
    StringTable::Key key = Key::from(m_table.size());
    auto allocated = std::make_unique<const std::string>(s);
    value_type_t value = allocated.get();
    m_table.push_back(std::move(allocated));
    return Entry { key, value };
}

StringTable::Entry StringTable::add(char c) { return add(std::string(1, c)); }

StringTable::value_type_t StringTable::get_at(Key key) const {
    ASSERT(contains(key));
    return m_table[key.id].get();
}

bool StringTable::contains(Key key) const { return key.id < m_table.size(); }

void StringTable::dump() const {
    for (size_t i = 0; i < m_table.size(); ++i) {
        auto& val = m_table[i];
        fmt::print("{}: \"{}\"\n", Key::from(i).to_string(), *val);
    }
}
