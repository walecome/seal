#include <iostream>

#include "IrProgram.hh"

void IrProgram::add_function(ptr_t<IrFunction>& function) {
    m_functions.push_back(std::move(function));
}

void IrProgram::dump() const {
    for (unsigned i = 0; i < m_functions.size(); ++i) {
        std::cout << "Dumping IR function #" << i + 1 << std::endl;
        m_functions.at(i)->dump_quads();
    }
}