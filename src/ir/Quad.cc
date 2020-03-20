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
    std::ostringstream oss {};

    unsigned indent_steps = 8;

    if (is_basic_block_entry) {
        oss << "BBE ";
    } else {
        oss << "    ";
    }

    indent_steps -= 4;

    if (has_label()) {
        oss << "L" << label_id() << ": ";
        indent_steps -= (nr_chars(label_id()) + 3);
    }

    ASSERT(indent_steps <= 8);

    oss << util::indent_spaces(indent_steps) << "("
        << opcode_to_string(m_op_code) << ", " << m_dest.to_string() << ", ";

    if (m_src_a.is_used()) {
        oss << m_src_a.to_string();
    } else {
        oss << "_";
    }

    oss << ", ";

    if (m_src_b.is_used()) {
        oss << m_src_b.to_string();
    } else {
        oss << "_";
    }

    oss << ")";

    return oss.str();
}