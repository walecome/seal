#include <fmt/format.h>

#include "Interpreter.hh"
#include "ir/IrFunction.hh"
#include "ir/IrProgram.hh"

void StackFrame::set_variable(const std::string_view identifier,
                              const Operand value) {
    auto target_frame = get_variable_frame(identifier);

    if (target_frame) {
        target_frame->assign_variable(identifier, value);
    } else {
        // No frame had the variable previously set
        m_variables[identifier] = value;
    }
}

void StackFrame::assign_variable(const std::string_view identifier,
                                 const Operand value) {
    ASSERT(m_variables.count(identifier) == 1);

    m_variables[identifier] = value;
}

Operand StackFrame::get_variable(const std::string_view identifier) {
    auto it = m_variables.find(identifier);

    if (it == std::end(m_variables)) {
        return m_parent->get_variable(identifier);
    }

    return it->second;
}

StackFrame* StackFrame::get_variable_frame(std::string_view identifier) {
    auto it = m_variables.find(identifier);

    if (it == std::end(m_variables)) {
        if (!m_parent) {
            return nullptr;
        }

        return m_parent->get_variable_frame(identifier);
    }

    return this;
}

void Interpreter::interpret() {
    interpret_function(
        m_ir_program->get_function_from_id(m_ir_program->main_function_id()));
}

void Interpreter::interpret_function(const IrFunction* function) {
    for (const Quad* quad : function->quads_as_pointers()) {
        switch (quad->opcode()) {
            case OPCode::ADD:
                add(quad);
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

template <class Operator>
struct BinOpVisitor {
    template <typename T, typename U>
    operand_type_t operator()(T, U) {
        ASSERT_NOT_REACHED();
    }

    operand_type_t operator()(StringOperand, StringOperand) {
        ASSERT_NOT_REACHED();
    }

    template <typename T>
    operand_type_t operator()(T a, T b) {
        return T { Operator {}(a, b) };
    }
};

void Interpreter::add(const Quad* quad) {
    ASSERT(quad->opcode() == OPCode::ADD);
    ASSERT(quad->dest().is_variable());

    operand_type_t result =
        std::visit(BinOpVisitor<std::plus<>> {}, quad->src_a().data(),
                   quad->src_b().data());
}

void Interpreter::sub(const Quad* quad) {
    ASSERT(quad->opcode() == OPCode::SUB);
    ASSERT(quad->dest().is_variable());

    operand_type_t result =
        std::visit(BinOpVisitor<std::minus<>> {}, quad->src_a().data(),
                   quad->src_b().data());
}
void Interpreter::mult(const Quad* quad) {
    ASSERT(quad->opcode() == OPCode::MULT);
    ASSERT(quad->dest().is_variable());

    operand_type_t result =
        std::visit(BinOpVisitor<std::multiplies<>> {}, quad->src_a().data(),
                   quad->src_b().data());
}

void Interpreter::div(const Quad* quad) {
    ASSERT(quad->opcode() == OPCode::MULT);
    ASSERT(quad->dest().is_variable());

    operand_type_t result =
        std::visit(BinOpVisitor<std::divides<>> {}, quad->src_a().data(),
                   quad->src_b().data());
}

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