#include <fmt/format.h>

#include "Interpreter.hh"
#include "builtin/BuiltIn.hh"
#include "ir/QuadCollection.hh"

Interpreter::Interpreter(const QuadCollection& quads, bool verbose)
    : m_quads { quads }, m_verbose { verbose } {}

void Interpreter::interpret() {
    m_stack_frames.push(StackFrame { 0, nullptr });
    interpret_function(m_quads.main_function_id);
}

void Interpreter::interpret_function(unsigned function_id) {
    if (m_verbose) {
        fmt::print("Interpreting function with id {}\n", function_id);
    }

    current_frame()->set_program_counter(
        m_quads.function_to_quad.at(function_id));

    while (true) {
        const Quad& quad = m_quads.quads[current_frame()->program_counter()];

        // fmt::print("Interpreting quad: {}\n", quad.to_string());

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
                ret(quad);
                break;
            case OPCode::MOVE:
                move(quad);
                break;
            case OPCode::INDEX_MOVE:
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
void binop_helper(StackFrame* context, const Quad& quad) {
    ASSERT(quad.dest().is_variable());

    ValueOperand lhs = context->resolve_operand(quad.src_a());
    ValueOperand rhs = context->resolve_operand(quad.src_b());

    value_operand_t result =
        std::visit(BinOpVisitor<Operator> {}, lhs.value, rhs.value);

    context->set_variable(quad.dest().as_variable(), ValueOperand { result });
}

void Interpreter::add(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::ADD);
    binop_helper<std::plus<>>(current_frame(), quad);
}

void Interpreter::sub(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::SUB);
    binop_helper<std::minus<>>(current_frame(), quad);
}
void Interpreter::mult(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::MULT);
    binop_helper<std::multiplies<>>(current_frame(), quad);
}

void Interpreter::div(const Quad& quad) {
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
void cmp_helper(StackFrame* context, const Quad& quad) {
    ValueOperand lhs = context->resolve_operand(quad.src_a());
    ValueOperand rhs = context->resolve_operand(quad.src_b());

    bool result = std::visit(CmpVisitor<Operator> {}, lhs.value, rhs.value);

    value_operand_t ret = IntOperand { result };
    context->set_variable(quad.dest().as_variable(), ValueOperand { ret });
}

void Interpreter::cmp_eq(const Quad& quad) {
    cmp_helper<std::equal_to<>>(current_frame(), quad);
}

void Interpreter::cmp_gt(const Quad& quad) {
    cmp_helper<std::greater<>>(current_frame(), quad);
}

void Interpreter::cmp_lt(const Quad& quad) {
    cmp_helper<std::less<>>(current_frame(), quad);
}

void Interpreter::cmp_gteq(const Quad& quad) {
    cmp_helper<std::greater_equal<>>(current_frame(), quad);
}

void Interpreter::cmp_lteq(const Quad& quad) {
    cmp_helper<std::less_equal<>>(current_frame(), quad);
}

void Interpreter::cmp_noteq(const Quad& quad) {
    cmp_helper<std::not_equal_to<>>(current_frame(), quad);
}
void Interpreter::jmp(const Quad& quad) {
    ASSERT(quad.dest().is_label());

    unsigned label_id = quad.dest().as_label();

    auto target_quad = m_quads.label_to_quad.find(label_id);
    ASSERT(target_quad != m_quads.label_to_quad.end());
    current_frame()->set_program_counter(target_quad->second);
    current_frame()->set_jump_performed(true);
}

void Interpreter::jmp_z(const Quad& quad) {
    ValueOperand condition = current_frame()->resolve_operand(quad.src_a());
    if (std::get<IntOperand>(condition.value) == 0) {
        jmp(quad);
    }
}

void Interpreter::jmp_nz(const Quad& quad) {
    ValueOperand condition = current_frame()->resolve_operand(quad.src_a());
    if (std::get<IntOperand>(condition.value) != 0) {
        jmp(quad);
    }
}

void Interpreter::push_arg(const Quad& quad) {
    // TODO: Built-ins will break from this as they do not have parameter names

    if (!m_arguments) {
        m_arguments = ArgumentWrapper {};
    }

    ValueOperand value = current_frame()->resolve_operand(quad.src_a());
    // This means we have a named argument
    if (quad.dest().is_used()) {
        m_arguments.value().add_named_argument(quad.dest().as_variable(),
                                               value);
    } else {
        m_arguments.value().add_argument(value);
    }
}

void Interpreter::call(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::CALL);
    ASSERT(quad.src_a().is_function());

    FunctionOperand func = quad.src_a().as_function();

    if (BuiltIn::is_builtin(func)) {
        BuiltIn::call_builtin_function(func, take_arguments().value_vector());
        return;
    }

    enter_new_frame();

    // Program counter will be incremented in interpret function
    current_frame()->set_program_counter(m_quads.function_to_quad.at(func) - 1);

    take_arguments().for_each_name_value(
        [&](VariableOperand var, ValueOperand value) {
            current_frame()->set_variable(var, value);
        });

    if (!quad.dest().is_used()) {
        return;
    }

    ASSERT(quad.dest().is_variable());

    current_frame()->set_return_variable(quad.dest().as_variable());
}

void Interpreter::ret(const Quad& quad) {
    if (!quad.dest().is_used()) {
        exit_frame();
        return;
    }

    ValueOperand value = current_frame()->resolve_operand(quad.dest());

    if (current_frame()->is_main_frame()) {
        exit(value.as_int());
    }

    if (current_frame()->return_variable()) {
        VariableOperand ret = current_frame()->return_variable().value();
        exit_frame();
        current_frame()->set_variable(ret, value);
    } else {
        exit_frame();
    }
}

void Interpreter::move(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::MOVE);

    ASSERT(quad.dest().is_variable());

    current_frame()->set_variable(
        quad.dest().as_variable(),
        current_frame()->resolve_operand(quad.src_a()));
}

void Interpreter::index_move(const Quad&) {}

StackFrame* Interpreter::current_frame() { return &m_stack_frames.top(); }

void Interpreter::enter_new_frame() {
    // fmt::print("enter_frame()\n");

    m_stack_frames.emplace(current_frame()->program_counter(), current_frame());
};

void Interpreter::exit_frame() {
    // fmt::print("exit_frame()\n");
    m_stack_frames.pop();
}

ArgumentWrapper Interpreter::take_arguments() {
    if (!m_arguments) {
        return {};
    }

    ArgumentWrapper arguments = m_arguments.value();
    m_arguments = std::nullopt;

    return arguments;
}
