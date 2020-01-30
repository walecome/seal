#include <iomanip>
#include <iostream>

#include "ast/FunctionDecl.hh"
#include "builtin/BuiltIn.hh"
#include "ir/IrProgram.hh"

void IrProgram::add_function(ptr_t<IrFunction>& function) {
    if (function->declaration()->identifier() == "main") {
        m_main_function_id = function->declaration()->function_id();
    }

    m_functions.push_back(std::move(function));
}

const IrFunction* IrProgram::get_function_from_id(unsigned id) const {
    unsigned builtin_offset = BuiltIn::number_of_builtins();
    ASSERT(id >= builtin_offset);
    return m_functions.at(id - builtin_offset).get();
}

void IrProgram::dump() const {
    for (auto& ir_function : m_functions) {
        std::cout << "Dumping IR for function "
                  << std::quoted(ir_function->declaration()->identifier())
                  << " with id " << ir_function->declaration()->function_id()
                  << std::endl;
        ir_function->dump_quads();

        // std::cout << "Function variables: " << std::endl;
        // ir_function->__dump_variables();

        std::cout << std::endl;
    }
}