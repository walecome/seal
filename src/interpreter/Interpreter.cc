#include <fmt/format.h>

#include "Interpreter.hh"
#include "ir/IrFunction.hh"
#include "ir/IrProgram.hh"

void Interpreter::interpret() {
    interpret_function(
        m_ir_program->get_function_from_id(m_ir_program->main_function_id()));
}

void Interpreter::interpret_function(const IrFunction* function) {
    for (const Quad* quad : function->quads_as_pointers()) {
        switch (quad->opcode()) {
            case OPCode::ADD:

                break;
            case OPCode::SUB:
                break;
            case OPCode::MULT:
                break;
            case OPCode::DIV:
                break;
            case OPCode::CMP_EQ:
                break;
            case OPCode::CMP_GT:
                break;
            case OPCode::CMP_LT:
                break;
            case OPCode::CMP_GTEQ:
                break;
            case OPCode::CMP_LTEQ:
                break;
            case OPCode::CMP_NOTEQ:
                break;
            case OPCode::JMP:
                break;
            case OPCode::JMP_Z:
                break;
            case OPCode::JMP_NZ:
                break;
            case OPCode::PUSH_ARG:
                break;
            case OPCode::CALL:
                break;
            case OPCode::RET:
                break;
            case OPCode::MOVE:
                break;
            case OPCode::INDEX_MOVE:
                break;

            default:
                ASSERT_NOT_REACHED();
        }
    }
}

void Interpreter::add(const Quad* quad) {}
void Interpreter::sub(const Quad* quad) {}
void Interpreter::mult(const Quad* quad) {}
void Interpreter::div(const Quad* quad) {}
void Interpreter::cmp_eq(const Quad* quad) {}
void Interpreter::cmp_gt(const Quad* quad) {}
void Interpreter::cmp_lt(const Quad* quad) {}
void Interpreter::cmp_gteq(const Quad* quad) {}
void Interpreter::cmp_lteq(const Quad* quad) {}
void Interpreter::cmp_noteq(const Quad* quad) {}
void Interpreter::jmp(const Quad* quad) {}
void Interpreter::jmp_z(const Quad* quad) {}
void Interpreter::jmp_nz(const Quad* quad) {}
void Interpreter::push_arg(const Quad* quad) {}
void Interpreter::call(const Quad* quad) {}
void Interpreter::ret(const Quad* quad) {}
void Interpreter::move(const Quad* quad) {}
void Interpreter::index_move(const Quad* quad) {}