#include <iomanip>
#include <iostream>
#include <type_traits>

#include "ast/FunctionDecl.hh"
#include "builtin/BuiltIn.hh"
#include "ir/IrProgram.hh"

void IrProgram::add_function(FunctionDecl* func_decl) {
    m_functions.push_back(std::make_unique<IrFunction>(func_decl));

    IrFunction& function = current_func();

    if (function.declaration()->identifier() == "main") {
        m_main_function_id = function.declaration()->function_id();
    }
}

const IrFunction& IrProgram::get_function_from_id(unsigned id) const {
    unsigned builtin_offset = BuiltIn::number_of_builtins();
    ASSERT(id >= builtin_offset);

    return *m_functions.at(id - builtin_offset);
}

void IrProgram::dump() const {
    for (auto& ir_function : m_functions) {
        std::cout << "Dumping IR for function "
                  << std::quoted(ir_function->declaration()->identifier())
                  << " with id " << ir_function->declaration()->function_id()
                  << std::endl;
        ir_function->print_blocks();

        std::cout << std::endl;
    }
}