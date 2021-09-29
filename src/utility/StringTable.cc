#include <utility/StringTable.hh>

StringTable::Entry StringTable::add(std::string&& s) {
    StringTable::Key key = Key::from(m_table.size());
    auto allocated = std::make_unique<const std::string>(s);
    value_type_t value = allocated.get();
    m_table.push_back(std::move(allocated));
    return Entry{ key, value };
}

StringTable::value_type_t StringTable::get_at(Key key) const {
    return m_table[key.id].get();
}
