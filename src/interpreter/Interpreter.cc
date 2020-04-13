#include <fmt/format.h>

#include "Interpreter.hh"
#include "builtin/BuiltIn.hh"
#include "ir/QuadCollection.hh"

Interpreter::Interpreter(const QuadCollection& quads) : m_quads { quads } {}

void Interpreter::interpret() {
    m_stack_frames.push(StackFrame {});
    interpret_function(m_quads.main_function_id);
}

void Interpreter::interpret_function(unsigned function_id) {
    fmt::print("Interpreting function with id {}\n", function_id);

    current_frame()->set_program_counter(
        m_quads.function_to_quad.at(function_id));

    while (true) {
        Quad quad = m_quads.quads[current_frame()->program_counter()];

        // fmt::print("Interpreting: {}\n", quad.to_string());

        switch (quad.opcode()) {
            case OPCode::ADD:
                add(quad);
                break;

            case OPCode::SUB:
                sub(quad);
                break;

            case OPCode::MULT:
                mult(quad);
                break;

            case OPCode::DIV:
                div(quad);
                break;

            case OPCode::CMP_EQ:
                cmp_eq(quad);
                break;

            case OPCode::CMP_GT:
                cmp_gt(quad);
                break;

            case OPCode::CMP_LT:
                cmp_lt(quad);
                break;

            case OPCode::CMP_GTEQ:
                cmp_gteq(quad);
                break;

            case OPCode::CMP_LTEQ:
                cmp_lteq(quad);
                break;

            case OPCode::CMP_NOTEQ:
                cmp_noteq(quad);
                break;
            case OPCode::JMP:
                jmp(quad);
                break;
            case OPCode::JMP_Z:
                jmp_z(quad);
                break;
            case OPCode::JMP_NZ:
                jmp_nz(quad);
                break;
            case OPCode::PUSH_ARG:
                push_arg(quad);
                break;
            case OPCode::CALL:
                call(quad);
                break;
            case OPCode::RET:
                break;
            case OPCode::MOVE:
                move(quad);
                break;
            case OPCode::INDEX_MOVE:
                break;
            case OPCode::PREPARE_FRAME:
                prepare_frame();
                break;

            default:
                ASSERT_NOT_REACHED();
        }

        if (current_frame()->jump_performed()) {
            current_frame()->set_jump_performed(false);
        } else {
            current_frame()->increment_program_counter();
        }
    }
}

template <class BinaryOperator>
struct BinOpVisitor {
    template <typename T, typename U>
    value_operand_t operator()(T, U) {
        ASSERT_NOT_REACHED();
    }

    value_operand_t operator()(StringOperand, StringOperand) {
        ASSERT_NOT_REACHED();
    }

    template <typename T>
    value_operand_t operator()(T a, T b) {
        return T { BinaryOperator {}(a, b) };
    }
};

template <class Operator>
void binop_helper(StackFrame* context, Quad quad) {
    ASSERT(quad.dest().is_variable());

    ValueOperand lhs = context->resolve_operand(quad.src_a());
    ValueOperand rhs = context->resolve_operand(quad.src_b());

    value_operand_t result =
        std::visit(BinOpVisitor<Operator> {}, lhs.value, rhs.value);

    context->set_variable(quad.dest().as_variable(), ValueOperand { result },
                          false);
}

void Interpreter::add(Quad quad) {
    ASSERT(quad.opcode() == OPCode::ADD);
    binop_helper<std::plus<>>(current_frame(), quad);
}

void Interpreter::sub(Quad quad) {
    ASSERT(quad.opcode() == OPCode::SUB);
    binop_helper<std::minus<>>(current_frame(), quad);
}
void Interpreter::mult(Quad quad) {
    ASSERT(quad.opcode() == OPCode::MULT);
    binop_helper<std::multiplies<>>(current_frame(), quad);
}

void Interpreter::div(Quad quad) {
    ASSERT(quad.opcode() == OPCode::DIV);
    binop_helper<std::divides<>>(current_frame(), quad);
}

template <class Operator>
struct CmpVisitor {
    template <typename T, typename U>
    bool operator()(T, U) {
        ASSERT_NOT_REACHED();
    }

    template <typename T>
    bool operator()(T a, T b) {
        return Operator {}(a.value, b.value);
    }
};

template <class Operator>
void cmp_helper(StackFrame* context, Quad quad) {
    ValueOperand lhs = context->resolve_operand(quad.src_a());
    ValueOperand rhs = context->resolve_operand(quad.src_b());

    bool result = std::visit(CmpVisitor<Operator> {}, lhs.value, rhs.value);

    value_operand_t ret = IntOperand { result };
    context->set_variable(quad.dest().as_variable(), ValueOperand { ret },
                          false);
}

void Interpreter::cmp_eq(Quad quad) {
    cmp_helper<std::equal_to<>>(current_frame(), quad);
}

void Interpreter::cmp_gt(Quad quad) {
    cmp_helper<std::greater<>>(current_frame(), quad);
}

void Interpreter::cmp_lt(Quad quad) {
    cmp_helper<std::less<>>(current_frame(), quad);
}

void Interpreter::cmp_gteq(Quad quad) {
    cmp_helper<std::greater_equal<>>(current_frame(), quad);
}

void Interpreter::cmp_lteq(Quad quad) {
    cmp_helper<std::less_equal<>>(current_frame(), quad);
}

void Interpreter::cmp_noteq(Quad quad) {
    cmp_helper<std::not_equal_to<>>(current_frame(), quad);
}
void Interpreter::jmp(Quad quad) {
    ASSERT(quad.dest().is_label());

    unsigned label_id = quad.dest().as_label();

    auto target_quad = m_quads.label_to_quad.find(label_id);
    ASSERT(target_quad != m_quads.label_to_quad.end());
    current_frame()->set_program_counter(target_quad->second);
    current_frame()->set_jump_performed(true);
}

void Interpreter::jmp_z(Quad quad) {
    ValueOperand condition = current_frame()->resolve_operand(quad.src_a());
    if (std::get<IntOperand>(condition.value) == 0) {
        jmp(quad);
    }
}

void Interpreter::jmp_nz(Quad quad) {
    ValueOperand condition = current_frame()->resolve_operand(quad.src_a());
    if (std::get<IntOperand>(condition.value) != 0) {
        jmp(quad);
    }
}

void Interpreter::push_arg(Quad quad) {
    // TODO: Built-ins will break from this as they do not have parameter names
    current_frame()->set_variable(
        quad.dest().as_variable(),
        current_frame()->resolve_operand(quad.src_a()), true);
}

void Interpreter::call(Quad quad) {
    ASSERT(quad.opcode() == OPCode::CALL);
    ASSERT(quad.src_a().is_function());

    // When we get here, the prepare_frame quad should have already been
    // interpreted. This means we simply have to set the program counter to
    // correct quad.

    FunctionOperand func = quad.src_a().as_function();

    if (BuiltIn::is_builtin(func)) {
        BuiltIn::call_builtin_function(func, current_frame()->get_variables());
        return;
    }

    current_frame()->set_program_counter(m_quads.function_to_quad.at(func));

    if (!quad.dest().is_used()) {
        return;
    }

    ASSERT(quad.dest().is_variable());

    current_frame()->set_return_variable(quad.dest().as_variable());
}

void Interpreter::ret(Quad quad) {
    if (current_frame()->return_variable()) {
        ASSERT(quad.dest().is_used());

        VariableOperand ret = current_frame()->return_variable().value();
        ValueOperand value = current_frame()->resolve_operand(quad.dest());
        exit_frame();

        current_frame()->set_variable(ret, value, false);
    } else {
        exit_frame();
    }
}

void Interpreter::move(Quad quad) {
    ASSERT(quad.opcode() == OPCode::MOVE);

    ASSERT(quad.dest().is_variable());

    current_frame()->set_variable(
        quad.dest().as_variable(),
        current_frame()->resolve_operand(quad.src_a()), false);
}

void Interpreter::index_move(Quad) {}

void Interpreter::prepare_frame() { enter_new_frame(); }