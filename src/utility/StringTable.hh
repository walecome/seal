#pragma once

#include <fmt/format.h>

#include <string>
#include <vector>
#include <memory>

class StringTable {

public:
    using value_type_t = std::string const *;

    struct Key {
        private:
            Key(size_t id) : id(id) {}

        public:
        static Key from(size_t id) {
            return Key(id);
        }
        const size_t id;

        std::string to_string() const {
            return fmt::format("s{}", id);
        }
    };

    struct Entry {
        const Key key;
        const value_type_t value;
    };

    StringTable() = default;

    // Add a string to the string table.
    // This is destructive operation for `s`, i.e.
    // s is not usable after a call to this method.
    Entry add(std::string& s);

    value_type_t get_at(Key key) const;

private:

    std::vector<std::unique_ptr<const std::string>> m_table{};
};
