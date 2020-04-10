#include "Quad.hh"
#include "Util.hh"

void Quad::add_label(unsigned label_id) {
    // BUG: We need to support a quad having more than one label
    m_has_label = true;

    m_label_ids.push_back(label_id);
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

std::string Quad::to_string() const {
    // TODO: Use fmtlib for this function...
    std::ostringstream oss {};

    unsigned indent_steps = 8;

    if (has_label()) {
        for (unsigned label_id : m_label_ids) {
            oss << "L" << label_id << " ";
            indent_steps -= (nr_chars(label_id) + 2);
        }
        oss << ": ";
        indent_steps -= 2;
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