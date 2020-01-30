#include <fmt/format.h>

#include "Interpreter.hh"
#include "ir/IrFunction.hh"
#include "ir/IrProgram.hh"

void Interpreter::interpret() {
    interpret_function(
        m_ir_program->get_function_from_id(m_ir_program->main_function_id()));
}

void Interpreter::interpret_function(const IrFunction* function) {
    for (const ptr_t<Quad>& quad : function->quads()) {
    }
}