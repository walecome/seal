#include <fmt/format.h>

#include "Interpreter.hh"
#include "builtin/BuiltIn.hh"
#include "ir/Generate.hh"

void StackFrame::set_variable(Operand var, ValueOperand value) {
    ASSERT(var.is_variable());

    VariableOperand var_raw = std::get<VariableOperand>(var.data());

    auto target_frame = get_variable_frame(var_raw);

    if (target_frame) {
        target_frame->assign_variable(var_raw, value);
    } else {
        // No frame had the variable previously set
        m_variables[var_raw] = value;
    }
}

void StackFrame::assign_variable(VariableOperand var, ValueOperand value) {
    ASSERT(m_variables.count(var) == 1);

    m_variables[var] = value;
}

ValueOperand StackFrame::get_variable(VariableOperand var) const {
    auto it = m_variables.find(var);

    if (it == std::end(m_variables)) {
        return m_parent->get_variable(var);
    }

    return it->second;
}

struct OperandResolver {
    const StackFrame* env;

    ValueOperand operator()(ValueOperand value_operand) {
        return value_operand;
    }

    ValueOperand operator()(VariableOperand var) {
        return env->get_variable(var);
    }

    template <class T>
    ValueOperand operator()(T) {
        ASSERT_NOT_REACHED();
    }
};

ValueOperand StackFrame::resolve_operand(Operand operand) const {
    return std::visit(OperandResolver { this }, operand.data());
}

void StackFrame::push_argument(ValueOperand value) {
    m_arguments.push_back(std::move(value));
}

void StackFrame::clear_arguments() { m_arguments.clear(); }

const std::vector<ValueOperand>& StackFrame::get_arguments() const {
    return m_arguments;
}

StackFrame* StackFrame::get_variable_frame(VariableOperand var) {
    auto it = m_variables.find(var);

    if (it == std::end(m_variables)) {
        if (!m_parent) {
            return nullptr;
        }

        return m_parent->get_variable_frame(var);
    }

    return this;
}

Interpreter::Interpreter(const QuadCollection& quads) : m_quads { quads } {}

void Interpreter::interpret() {
    m_stack_frames.push(StackFrame {});
    interpret_function(m_quads.main_function_id);
}

void Interpreter::interpret_function(unsigned function_id) {
    fmt::print("Interpreting function with id {}\n", function_id);

    current_frame()->set_program_counter(
        m_quads.function_to_quad.at(function_id));

    while (m_current_quad_idx < m_quads.quads.size()) {
        Quad quad = m_quads.quads[m_current_quad_idx];

        m_jump_performed = false;

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

            default:
                ASSERT_NOT_REACHED();
        }

        if (!m_jump_performed) {
            ++m_current_quad_idx;
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

    context->set_variable(quad.dest(), ValueOperand { result });
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
    context->set_variable(quad.dest(), ValueOperand { ret });
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

    m_jump_performed = true;

    unsigned label_id = std::get<LabelOperand>(quad.dest().data()).value;
    m_current_quad_idx = m_quads.label_to_quad.at(label_id);
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
    current_frame()->push_argument(
        current_frame()->resolve_operand(quad.src_a()));
}

void Interpreter::call(Quad quad) {
    ASSERT(quad.opcode() == OPCode::CALL);
    ASSERT(quad.src_a().is_function());

    FunctionOperand func = std::get<FunctionOperand>(quad.src_a().data());

    if (BuiltIn::is_builtin(func)) {
        BuiltIn::call_builtin_function(func, current_frame()->get_arguments());
    } else {
        ASSERT_NOT_REACHED_MSG("Function calls not implemented yet");
    };

    current_frame()->clear_arguments();
}

void Interpreter::ret(Quad) {}

void Interpreter::move(Quad quad) {
    ASSERT(quad.opcode() == OPCode::MOVE);
    current_frame()->set_variable(
        quad.dest(), current_frame()->resolve_operand(quad.src_a()));
}
void Interpreter::index_move(Quad) {}
