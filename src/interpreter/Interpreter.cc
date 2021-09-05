#include <fmt/format.h>
#include <string_view>
#include <vector>

#include "Constants.hh"
#include "CTypeWrapper.hh"
#include "Interpreter.hh"
#include "OPCode.hh"
#include "builtin/BuiltIn.hh"
#include "dynlib/DynLibLoader.hh"
#include "dynlib/DynLib.hh"
#include "fmt/core.h"
#include "ir/QuadCollection.hh"

void runtime_error(const std::string& message) {
    fmt::print("{}\n", message);
    exit(EXIT_FAILURE);
}

Interpreter::Interpreter(const QuadCollection& quads, bool verbose)
    : m_quads { quads },
      m_registers(std::vector<Operand>(quads.register_count)),
      m_verbose { verbose } {}

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
            case OPCode::POP_ARG:
                pop_arg(quad);
                break;
            case OPCode::SAVE:
                save(quad);
                break;
            case OPCode::RESTORE:
                restore(quad);
                break;
            case OPCode::CALL:
                call(quad);
                break;
            case OPCode::CALL_C:
                call_c(quad);
                break;
            case OPCode::RET:
                ret(quad);
                break;
            case OPCode::MOVE:
                move(quad);
                break;
            case OPCode::INDEX_MOVE:
                index_move(quad);
                break;

            case OPCode::INDEX_ASSIGN:
                index_assign(quad);
                break;

            case OPCode::AND:
                interpret_and(quad);
                break;

            case OPCode::OR:
                interpret_or(quad);
                break;

            default:
                ASSERT_NOT_REACHED_MSG(
                    fmt::format("Invalid OPCode: {}",
                                opcode_to_string(quad.opcode()))
                        .c_str());
        }

        if (current_frame()->jump_performed()) {
            current_frame()->set_jump_performed(false);
        } else {
            current_frame()->increment_program_counter();
        }
    }
}

Operand Interpreter::resolve_register(Register reg) const {
    return m_registers[reg.index()];
}

void Interpreter::set_register(Register reg, Operand operand) {
    m_registers[reg.index()] = std::move(operand);
}

unsigned Interpreter::resolve_label(const QuadDest& dest) const {
    return dest.as_label().value;
}

ValueOperand Interpreter::resolve_source(const QuadSource& source) const {
    // TODO: Support functions
    ASSERT(!source.is_function());
    if (source.is_value()) {
        return source.as_value();
    }

    if (source.is_register()) {
        return resolve_register(source.as_register()).as_value();
    }

    ASSERT_NOT_REACHED_MSG(
        fmt::format("Invalid QuadSource type: {}", source.to_string()).c_str());
}

template <class BinaryOperator>
struct BinOpVisitor {
    template <typename T, typename U>
    ValueOperand operator()(T, U) {
        ASSERT_NOT_REACHED();
    }

    ValueOperand operator()(StringOperand, StringOperand) {
        ASSERT_NOT_REACHED();
    }

    ValueOperand operator()(VectorOperand, VectorOperand) {
        ASSERT_NOT_REACHED();
    }

    template <typename T>
    ValueOperand operator()(T a, T b) {
        return ValueOperand { T { BinaryOperator {}(a, b) } };
    }
};

template <class Operator>
void binop_helper(Interpreter* interpreter, const Quad& quad) {
    ValueOperand lhs = interpreter->resolve_source(quad.src_a());
    ValueOperand rhs = interpreter->resolve_source(quad.src_b());

    Operand result =
        Operand(std::visit(BinOpVisitor<Operator> {}, lhs.value, rhs.value));

    interpreter->set_register(quad.dest().as_register(), result);
}

void Interpreter::add(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::ADD);
    binop_helper<std::plus<>>(this, quad);
}

void Interpreter::sub(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::SUB);
    binop_helper<std::minus<>>(this, quad);
}
void Interpreter::mult(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::MULT);
    binop_helper<std::multiplies<>>(this, quad);
}

void Interpreter::div(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::DIV);
    binop_helper<std::divides<>>(this, quad);
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
void cmp_helper(Interpreter* interpreter, const Quad& quad) {
    ValueOperand lhs = interpreter->resolve_source(quad.src_a());
    ValueOperand rhs = interpreter->resolve_source(quad.src_b());

    bool result = std::visit(CmpVisitor<Operator> {}, lhs.value, rhs.value);

    Operand ret = Operand { ValueOperand { IntOperand { result } } };
    interpreter->set_register(quad.dest().as_register(), ret);
}

void Interpreter::cmp_eq(const Quad& quad) {
    cmp_helper<std::equal_to<>>(this, quad);
}

void Interpreter::cmp_gt(const Quad& quad) {
    cmp_helper<std::greater<>>(this, quad);
}

void Interpreter::cmp_lt(const Quad& quad) {
    cmp_helper<std::less<>>(this, quad);
}

void Interpreter::cmp_gteq(const Quad& quad) {
    cmp_helper<std::greater_equal<>>(this, quad);
}

void Interpreter::cmp_lteq(const Quad& quad) {
    cmp_helper<std::less_equal<>>(this, quad);
}

void Interpreter::cmp_noteq(const Quad& quad) {
    cmp_helper<std::not_equal_to<>>(this, quad);
}
void Interpreter::jmp(const Quad& quad) {
    unsigned label_id = resolve_label(quad.dest());

    auto target_quad = m_quads.label_to_quad.find(label_id);
    ASSERT(target_quad != m_quads.label_to_quad.end());
    current_frame()->set_program_counter(target_quad->second);
    current_frame()->set_jump_performed(true);
}

void Interpreter::jmp_z(const Quad& quad) {
    ValueOperand condition = resolve_source(quad.src_a());
    if (condition.as_int() == 0) {
        jmp(quad);
    }
}

void Interpreter::jmp_nz(const Quad& quad) {
    ValueOperand condition = resolve_source(quad.src_a());
    if (condition.as_int() != 0) {
        jmp(quad);
    }
}

void Interpreter::push_arg(const Quad& quad) {
    // TODO: Built-ins will break from this as they do not have parameter names

    ValueOperand value = resolve_source(quad.src_a());
    m_arguments.push(ArgumentWrapper { value });
}

void Interpreter::pop_arg(const Quad& quad) {
    ASSERT(!m_arguments.empty());

    ArgumentWrapper argument = m_arguments.front();
    m_arguments.pop();

    set_register(quad.dest().as_register(), Operand { argument.value });
}

void Interpreter::save(const Quad& quad) {
    int start_idx = quad.src_a().as_register().index();
    int end_index = quad.src_b().as_register().index();

    for (int i = start_idx; i <= end_index; ++i) {
        m_stack.push(m_registers.at(i));
    }
}

void Interpreter::restore(const Quad& quad) {
    int start_idx = quad.src_a().as_register().index();
    int end_index = quad.src_b().as_register().index();
    for (int i = end_index; i >= start_idx; --i) {
        set_register(Register(i), m_stack.top());
        m_stack.pop();
    }
}

void Interpreter::call(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::CALL);

    FunctionOperand func = quad.src_a().as_function();

    if (BuiltIn::is_builtin(func)) {
        std::vector<ValueOperand> args {};
        while (!m_arguments.empty()) {
            args.push_back(m_arguments.front().value);
            m_arguments.pop();
        }
        ValueOperand ret = BuiltIn::call_builtin_function(func, args);
        set_register(quad.dest().as_register(), Operand { ret });
        return;
    }

    enter_new_frame();

    // Program counter will be incremented in interpret function
    current_frame()->set_program_counter(m_quads.function_to_quad.at(func) - 1);
}

void Interpreter::call_c(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::CALL_C);
    
    std::vector<ValueOperand> args {};
    while (!m_arguments.empty()) {
        args.push_back(m_arguments.front().value);
        m_arguments.pop();
    }
    
    std::string_view lib = quad.src_a().as_value().as_string();
    std::string_view func = quad.src_b().as_value().as_string();

    call_c_func(lib, func, args);
}

void Interpreter::ret(const Quad& quad) {
    auto source = quad.src_a();

    if (current_frame()->is_main_frame()) {
        ValueOperand value = resolve_source(QuadSource { Register(0) });
        exit(value.as_int());
    }

    if (current_frame()->return_variable()) {
        exit_frame();
        set_register(Register(0), Operand { resolve_source(source) });
    } else {
        exit_frame();
    }
}

void Interpreter::move(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::MOVE);
    set_register(quad.dest().as_register(),
                 Operand { resolve_source(quad.src_a()) });
}

void bounds_check(VectorOperand::value_type_t vec, int index) {
    if (index < 0) {
        runtime_error(
            fmt::format("Cannot index vector with negative index [{}]", index));
    }

    if (index >= static_cast<int>(vec->size())) {
        runtime_error(
            fmt::format("Index out-of-bounds error. Error indexing vector of "
                        "size {} with index {}",
                        vec->size(), index));
    }
}

void Interpreter::index_move(const Quad& quad) {
    int index = resolve_source(quad.src_b()).as_int();
    VectorOperand::value_type_t indexed =
        resolve_source(quad.src_a()).as_vector();

    bounds_check(indexed, index);

    ValueOperand value = indexed->at(index);
    set_register(quad.dest().as_register(), Operand { value });
}

void Interpreter::index_assign(const Quad& quad) {
    int index = resolve_source(quad.src_a()).as_int();
    VectorOperand::value_type_t indexed =
        resolve_source(QuadSource { quad.dest().as_register() }).as_vector();

    ValueOperand value = resolve_source(quad.src_b());

    bounds_check(indexed, index);
    indexed->at(index) = value;
}

void Interpreter::interpret_and(const Quad& quad) {
    ValueOperand lhs = resolve_source(quad.src_a());
    ValueOperand rhs = resolve_source(quad.src_b());

    Operand result = Operand { ValueOperand {
        IntOperand { lhs.as_int() && rhs.as_int() } } };

    set_register(quad.dest().as_register(), result);
}

void Interpreter::interpret_or(const Quad& quad) {
    ValueOperand lhs = resolve_source(quad.src_a());
    ValueOperand rhs = resolve_source(quad.src_b());

    Operand result = Operand { ValueOperand {
        IntOperand { lhs.as_int() || rhs.as_int() } } };

    set_register(quad.dest().as_register(), result);
}

StackFrame* Interpreter::current_frame() { return &m_stack_frames.top(); }

void Interpreter::enter_new_frame() {
    m_stack_frames.emplace(current_frame()->program_counter(), current_frame());
};

void Interpreter::exit_frame() {
    // fmt::print("exit_frame()\n");
    m_stack_frames.pop();
}

void Interpreter::call_c_func(std::string_view lib, std::string_view func, const std::vector<ValueOperand>& args) {
    Result<dynlib::DynamicLibrary*> loaded_lib_or_error = dynlib::load_lib(std::string(lib));
    if (loaded_lib_or_error.is_error()) {
        ASSERT_NOT_REACHED_MSG("Could not load expected library");
    }

    dynlib::DynamicLibrary* loaded_lib = loaded_lib_or_error.get();
    ASSERT(loaded_lib->has_symbol(std::string(func)));
    dynlib::DynamicLibrary::Callable callable = loaded_lib->get_callable(std::string(func));
    
    std::vector<ptr_t<vm::CTypeWrapper>> wrapped_args;
    for (const ValueOperand& op : args) {
        wrapped_args.push_back(vm::CTypeWrapper::from(op));
    }
    
    auto arg = [&] (int idx) {
        return wrapped_args[idx]->to_arg();
    };

    switch (wrapped_args.size()) {
        case 0:
            callable.call();
            break;
        case 1:
            callable.call(arg(0));
            break;
        case 2:
            callable.call(arg(0), arg(1));
            break;

        default:
            ASSERT_NOT_REACHED_MSG(fmt::format("Unsupported number of arguments passed to C function: {}", wrapped_args.size()));
    }
}