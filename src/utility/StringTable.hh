#pragma once

#include <fmt/format.h>

#include <memory>
#include <string>
#include <vector>
#include <string_view>

class StringTable {
   public:
    struct Key {
        Key() : id(0) {}
        Key(size_t id) : id(id) {}

        Key(const Key& other) { id = other.id; }

        Key& operator=(const Key& other) {
            id = other.id;
            return *this;
        }

       public:
        static Key from(size_t id) { return Key(id); }
        size_t id;

        std::string to_string() const { return fmt::format("s{}", id); }
    };

    struct Entry {
        const Key key;
        const std::string_view value;
    };

    StringTable() = default;

    // Add a string to the string table.
    // This is destructive operation for `s`, i.e.
    // s is not usable after a call to this method.
    Entry add(std::string&& s);

    Entry add(char c);

    std::string_view get_at(Key key) const;

    void dump() const;

   private:
    bool contains(Key key) const;

    std::vector<std::unique_ptr<const std::string>> m_table {};
};
