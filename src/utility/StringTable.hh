#ifndef STRINGTABLE_H_
#define STRINGTABLE_H_

#include <string>
#include <vector>

class StringTable {

public:
    struct Key {
        private:
            Key(size_t id) : id(id) {}

        public:
        static Key from(size_t id) {
            return Key(id);
        }
        const size_t id;
    };

    StringTable() = default;

    Key add(std::string& s);
    void replace_at(Key key, std::string& s);
    const std::string get_at(Key key) const;

private:

    std::string& at(Key key);

    std::vector<std::string> m_table{};

};

#endif // STRINGTABLE_H_
