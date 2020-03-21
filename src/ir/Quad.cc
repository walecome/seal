#include <fmt/format.h>

#include "Quad.hh"
#include "Util.hh"

void Quad::set_label(unsigned label_id) {
    ASSERT(!m_has_label);

    m_has_label = true;
    m_label_id = label_id;
}

unsigned nr_chars(unsigned number) {
    if (number < 10) {
        return 1;
    }

    unsigned count = 1;

    while (number >= 10) {
        number /= 10;

        count++;
    }

    return count;
}

std::string Quad::to_string(bool is_basic_block_entry) const {
    std::string bbe_str = is_basic_block_entry ? "BBE" : "";
    std::string label_str = has_label() ? fmt::format("L{}:", label_id()) : "";
    std::string opcode_str = opcode_to_string(m_op_code);
    std::string dest_str = m_dest.to_string();
    std::string src_a_str = m_src_a.is_used() ? m_src_a.to_string() : "_";
    std::string src_b_str = m_src_b.is_used() ? m_src_b.to_string() : "_";

    return fmt::format("{:<4} {:<4} {:<10}|{:>16}|{:>16}|{:>16}", bbe_str,
                       label_str, opcode_str, dest_str, src_a_str, src_b_str);
}