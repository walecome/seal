#include "Quad.hh"

std::string Quad::to_string() const {
    std::ostringstream oss {};

    oss << "(" << opcode_to_string(m_op_code) << ", " << m_dest.to_string()
        << ", " << m_src_a.to_string() << ", " << m_src_b.to_string() << ")";

    return oss.str();
}