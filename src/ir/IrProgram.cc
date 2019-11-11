#include "IrProgram.hh"

void IrProgram::add_function(ptr_t<IrFunction>&& function) {
    m_functions.push_back(std::move(function));
}