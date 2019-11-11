#include <iostream>

#include "IrFunction.hh"

void IrFunction::add_quad(ptr_t<Quad>&& quad) {
    m_quads.push_back(std::move(quad));
}

void IrFunction::dump_quads() const {
    for (auto& quad : m_quads) {
        std::cout << quad->to_string() << std::endl;
    }
}