#include <iomanip>
#include <iostream>

#include "ast/FunctionDecl.hh"
#include "ir/IrProgram.hh"

void IrProgram::add_function(ptr_t<IrFunction>& function) {
    m_functions.push_back(std::move(function));
}

void IrProgram::dump() const {
    for (auto& ir_function : m_functions) {
        std::cout << "Dumping IR for function "
                  << std::quoted(ir_function->declaration()->function_name())
                  << " with id " << ir_function->declaration()->function_id()
                  << std::endl;
        ir_function->dump_quads();

        std::cout << "Function variables: " << std::endl;
        ir_function->__dump_variables();

        std::cout << std::endl;
    }
}