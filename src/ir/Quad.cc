#include "Quad.hh"

std::string Quad::to_string() const {
    std::ostringstream oss {};

    oss << "(" << opcode_to_string(m_op_code) << ", " << m_dest.to_string()
        << ", ";

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