#include <utility/StringTable.hh>

StringTable::Key StringTable::add(std::string& s) {
    StringTable::Key key = Key::from(m_table.size());
    m_table.push_back(std::move(s));
    return key;
}

void StringTable::replace_at(Key key, std::string &s) {
    at(key) = std::move(s);
}

const std::string StringTable::get_at(Key key) const {
    return m_table[key.id];
}

std::string& StringTable::at(Key key) {
    return m_table[key.id];
}
