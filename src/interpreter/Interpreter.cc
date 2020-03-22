#include <fmt/format.h>

#include "Interpreter.hh"
#include "builtin/BuiltIn.hh"
#include "ir/IrFunction.hh"
#include "ir/IrProgram.hh"

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

void Interpreter::interpret() {
    const IrFunction& main_func =
        m_ir_program->get_function_from_id(m_ir_program->main_function_id());

    interpret_function(main_func);
}

void Interpreter::interpret_function(const IrFunction& function) {
    fmt::print("Interpreting function with id {}\n", function.id());

    m_call_stack.push(&function);

    m_current_quad_idx = 0;

    auto quad_ptrs = function->quads_as_pointers();

    while (m_current_quad_idx < quad_ptrs.size()) {
        const Quad& quad = quad_ptrs[m_current_quad_idx];
        m_jump_performed = false;

        switch (quad->opcode()) {
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
void binop_helper(StackFrame* context, const Quad& quad) {
    ASSERT(quad->dest().is_variable());

    ValueOperand lhs = context->resolve_operand(quad->src_a());
    ValueOperand rhs = context->resolve_operand(quad->src_b());

    value_operand_t result =
        std::visit(BinOpVisitor<Operator> {}, lhs.value, rhs.value);

    context->set_variable(quad->dest(), ValueOperand { result });
}

void Interpreter::add(const Quad& quad) {
    ASSERT(quad->opcode() == OPCode::ADD);
    binop_helper<std::plus<>>(current_frame(), quad);
}

void Interpreter::sub(const Quad& quad) {
    ASSERT(quad->opcode() == OPCode::SUB);
    binop_helper<std::minus<>>(current_frame(), quad);
}
void Interpreter::mult(const Quad& quad) {
    ASSERT(quad->opcode() == OPCode::MULT);
    binop_helper<std::multiplies<>>(current_frame(), quad);
}

void Interpreter::div(const Quad& quad) {
    ASSERT(quad->opcode() == OPCode::DIV);
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
    ValueOperand lhs = context->resolve_operand(quad->src_a());
    ValueOperand rhs = context->resolve_operand(quad->src_b());

    bool result = std::visit(CmpVisitor<Operator> {}, lhs.value, rhs.value);

    value_operand_t ret = IntOperand { result };
    context->set_variable(quad->dest(), ValueOperand { ret });
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
    ASSERT(quad->dest().is_label());

    m_jump_performed = true;

    // REFACTOR: These types of call chains can be shortened with API
    // improvements
    m_current_quad_idx = m_call_stack.top()->quad_idx(
        std::get<LabelOperand>(quad->dest().data()));
}

void Interpreter::jmp_z(const Quad& quad) {
    ValueOperand condition = current_frame()->resolve_operand(quad->src_a());
    if (std::get<IntOperand>(condition.value) == 0) {
        jmp(quad);
    }
}

void Interpreter::jmp_nz(const Quad& quad) {
    ValueOperand condition = current_frame()->resolve_operand(quad->src_a());
    if (std::get<IntOperand>(condition.value) != 0) {
        jmp(quad);
    }
}

void Interpreter::push_arg(const Quad& quad) {
    current_frame()->push_argument(
        current_frame()->resolve_operand(quad->src_a()));
}

void Interpreter::call(const Quad& quad) {
    ASSERT(quad->opcode() == OPCode::CALL);
    ASSERT(quad->src_a().is_function());

    FunctionOperand func = std::get<FunctionOperand>(quad->src_a().data());

    if (BuiltIn::is_builtin(func)) {
        BuiltIn::call_builtin_function(func, current_frame()->get_arguments());
    } else {
        ASSERT_NOT_REACHED();
    };

    current_frame()->clear_arguments();
}

void Interpreter::ret(const Quad&) {}
void Interpreter::move(const Quad& quad) {
    ASSERT(quad->opcode() == OPCode::MOVE);
    current_frame()->set_variable(
        quad->dest(), current_frame()->resolve_operand(quad->src_a()));
}
void Interpreter::index_move(const Quad&) {}
