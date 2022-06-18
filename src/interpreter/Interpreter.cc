#include <array>
#include <string_view>
#include <vector>

#include <fmt/format.h>

#include "Constants.hh"
#include "CrashHelper.hh"

#include "ir/OPCode.hh"

#include "builtin/BuiltIn.hh"

#include "dynlib/DynLib.hh"
#include "dynlib/DynLibLoader.hh"

#include "types/CType.hh"

#include "common/ConstantPool.hh"
#include "common/RelocatedQuad.hh"

#include "interpreter/CTypeWrapper.hh"
#include "interpreter/FunctionResolver.hh"
#include "interpreter/InstructionSequencer.hh"
#include "interpreter/Interpreter.hh"
#include "interpreter/LabelResolver.hh"
#include "interpreter/RegisterWindow.hh"

namespace {

void runtime_error(const std::string& message) {
    fmt::print("{}\n", message);
    exit(EXIT_FAILURE);
}

Register return_register() {
    return Register(0);
}

}  // namespace

Interpreter::Interpreter(InstructionSequencer* instruction_sequencer,
                         const ConstantPool* constant_pool, bool verbose)
    : m_instruction_sequencer(instruction_sequencer),
      m_constant_pool(constant_pool),
      m_register_windows(std::stack<RegisterWindow>()),
      m_verbose(verbose) {
}

void Interpreter::interpret() {
    CrashHelper::ScopedHandler handler([this] { handle_crash(); });

    while (true) {
        interpret_quad(sequencer().fetch_next_instruction());
    }
}

Value Interpreter::resolve_register(Register reg) const {
    Value value = current_register_window().get_from_register(reg);
    ASSERT(!value.is_none());
    return value;
}

Value Interpreter::resolve_to_value(const Operand& source) const {
    ASSERT(source.is_used());
    ASSERT(!source.is_address());

    if (source.is_constant()) {
        return constant_pool().get_value(source.as_constant());
    }

    if (source.is_register()) {
        return resolve_register(source.as_register());
    }

    ASSERT_NOT_REACHED_MSG(
        fmt::format("Invalid RelocatedQuad type: {}", source.to_debug_string())
            .c_str());
}

void Interpreter::set_register(Register reg, Value value) {
    current_register_window().set_register_value(reg, value);
}

namespace {
template <class BinaryOperator>
Value compute_binary_expression(const Value& lhs, const Value& rhs) {
    ASSERT(lhs.is_same_type(rhs));

    auto apply_binary_operator = [&](const auto& a, const auto& b) {
        return BinaryOperator {}(a, b);
    };

    if (lhs.is_boolean()) {
        bool result = apply_binary_operator(lhs.as_boolean().value(),
                                            rhs.as_boolean().value());
        return Value::create_boolean(result);
    }

    if (lhs.is_integer()) {
        int result = apply_binary_operator(lhs.as_integer().value(),
                                           rhs.as_integer().value());
        return Value::create_integer(result);
    }

    if (lhs.is_real()) {
        double result =
            apply_binary_operator(lhs.as_real().value(), rhs.as_real().value());
        return Value::create_real(result);
    }

    ASSERT_NOT_REACHED();
}

Value concatenate_strings(const String& lhs, const String& rhs) {
    ASSERT_NOT_REACHED_MSG("concatenate_strings() not implemented");
}

template <>
Value compute_binary_expression<std::plus<>>(const Value& lhs,
                                             const Value& rhs) {
    ASSERT(lhs.is_same_type(rhs));

    auto apply_binary_operator = [&](const auto& a, const auto& b) {
        return std::plus<> {}(a, b);
    };

    if (lhs.is_boolean()) {
        bool result = apply_binary_operator(lhs.as_boolean().value(),
                                            rhs.as_boolean().value());
        return Value::create_boolean(result);
    }

    if (lhs.is_integer()) {
        int result = apply_binary_operator(lhs.as_integer().value(),
                                           rhs.as_integer().value());
        return Value::create_integer(result);
    }

    if (lhs.is_real()) {
        double result =
            apply_binary_operator(lhs.as_real().value(), rhs.as_real().value());
        return Value::create_real(result);
    }

    if (lhs.is_object() && lhs.is_string()) {
        return concatenate_strings(lhs.as_string(), rhs.as_string());
    }

    ASSERT_NOT_REACHED();
}

}  // namespace

void Interpreter::add(const RelocatedQuad& quad) {
    ASSERT(quad.opcode() == OPCode::ADD);

    auto lhs = resolve_to_value(quad.src_a());
    auto rhs = resolve_to_value(quad.src_b());
    auto result = compute_binary_expression<std::plus<>>(lhs, rhs);

    set_register(quad.dest().as_register(), result);
}

void Interpreter::sub(const RelocatedQuad& quad) {
    ASSERT(quad.opcode() == OPCode::SUB);

    auto lhs = resolve_to_value(quad.src_a());
    auto rhs = resolve_to_value(quad.src_b());
    auto result = compute_binary_expression<std::minus<>>(lhs, rhs);

    set_register(quad.dest().as_register(), std::move(result));
}

void Interpreter::mult(const RelocatedQuad& quad) {
    ASSERT(quad.opcode() == OPCode::MULT);

    auto lhs = resolve_to_value(quad.src_a());
    auto rhs = resolve_to_value(quad.src_b());
    auto result = compute_binary_expression<std::multiplies<>>(lhs, rhs);

    set_register(quad.dest().as_register(), std::move(result));
}

void Interpreter::div(const RelocatedQuad& quad) {
    ASSERT(quad.opcode() == OPCode::DIV);

    auto lhs = resolve_to_value(quad.src_a());
    auto rhs = resolve_to_value(quad.src_b());
    auto result = compute_binary_expression<std::divides<>>(lhs, rhs);

    set_register(quad.dest().as_register(), std::move(result));
}

void Interpreter::compare(
    const RelocatedQuad& quad,
    std::function<bool(const Value&, const Value&)> comparison_predicate) {
    auto lhs = resolve_to_value(quad.src_a());
    auto rhs = resolve_to_value(quad.src_b());

    bool result = comparison_predicate(lhs, rhs);
    set_register(quad.dest().as_register(), Value::create_boolean(result));
}

void Interpreter::cmp_eq(const RelocatedQuad& quad) {
    compare(quad,
            [](const Value& lhs, const Value& rhs) { return lhs == rhs; });
}

void Interpreter::cmp_gt(const RelocatedQuad& quad) {
    compare(quad, [](const Value& lhs, const Value& rhs) { return lhs > rhs; });
}

void Interpreter::cmp_lt(const RelocatedQuad& quad) {
    compare(quad, [](const Value& lhs, const Value& rhs) { return lhs < rhs; });
}

void Interpreter::cmp_gteq(const RelocatedQuad& quad) {
    compare(quad,
            [](const Value& lhs, const Value& rhs) { return lhs >= rhs; });
}

void Interpreter::cmp_lteq(const RelocatedQuad& quad) {
    compare(quad,
            [](const Value& lhs, const Value& rhs) { return lhs <= rhs; });
}

void Interpreter::cmp_noteq(const RelocatedQuad& quad) {
    compare(quad,
            [](const Value& lhs, const Value& rhs) { return lhs != rhs; });
}

void Interpreter::jmp(const RelocatedQuad& quad) {
    InstructionAddress jump_address = quad.dest().as_address();
    sequencer().set_jump_address(jump_address);
}

void Interpreter::jmp_z(const RelocatedQuad& quad) {
    const Value& condition = resolve_to_value(quad.src_a());
    if (!condition.as_boolean().value()) {
        jmp(quad);
    }
}

void Interpreter::jmp_nz(const RelocatedQuad& quad) {
    const Value& condition = resolve_to_value(quad.src_a());
    if (!condition.as_boolean().value()) {
        jmp(quad);
    }
}

void Interpreter::push_arg(const RelocatedQuad& quad) {
    m_arguments.push(Value::copy(resolve_to_value(quad.src_a())));
}

void Interpreter::pop_arg(const RelocatedQuad& quad) {
    ASSERT(!m_arguments.empty());

    Value argument = std::move(m_arguments.front());
    m_arguments.pop();

    set_register(quad.dest().as_register(), std::move(argument));
}

void Interpreter::call(const RelocatedQuad& quad) {
    ASSERT(quad.opcode() == OPCode::CALL);

    if (quad.src_a().is_builtin()) {
        std::vector<Value> args {};
        while (!m_arguments.empty()) {
            args.push_back(std::move(m_arguments.front()));
            m_arguments.pop();
        }
        Value ret =
            BuiltIn::call_builtin_function(quad.src_a().as_builtin(), args);
        set_register(quad.dest().as_register(), std::move(ret));
        return;
    }

    sequencer().call(quad.src_a().as_address());
}

void Interpreter::call_c(const RelocatedQuad& quad) {
    ASSERT(quad.opcode() == OPCode::CALL_C);

    std::vector<Value> args {};
    while (!m_arguments.empty()) {
        args.push_back(std::move(m_arguments.front()));
        m_arguments.pop();
    }

    auto return_value =
        call_c_func(resolve_to_value(quad.src_a()).as_string().value(),
                    resolve_to_value(quad.src_b()).as_string().value(), args,
                    take_pending_type_id());

    if (return_value.has_value()) {
        set_register(quad.dest().as_register(), std::move(*return_value));
    }
}

void Interpreter::set_ret_type(const RelocatedQuad& quad) {
    ASSERT(quad.opcode() == OPCode::SET_RET_TYPE);
    set_pending_type_id(resolve_to_value(quad.src_a()).as_integer().value());
}

InstructionSequencer& Interpreter::sequencer() {
    return *m_instruction_sequencer;
}

const ConstantPool& Interpreter::constant_pool() const {
    return *m_constant_pool;
}

RegisterWindow& Interpreter::current_register_window() {
    ASSERT(!m_register_windows.empty());
    return m_register_windows.top();
}

const RegisterWindow& Interpreter::current_register_window() const {
    ASSERT(!m_register_windows.empty());
    return m_register_windows.top();
}

void Interpreter::ret(const RelocatedQuad&) {
    if (sequencer().is_in_main_function()) {
        const Value& value = resolve_register(return_register());
        exit(value.as_integer().value());
    }

    ASSERT(!m_register_windows.empty());
    m_register_windows.pop();
    sequencer().ret();
}

void Interpreter::move(const RelocatedQuad& quad) {
    ASSERT(quad.opcode() == OPCode::MOVE);
    Value value = resolve_to_value(quad.src_a());
    set_register(quad.dest().as_register(), Value::copy(value));
}

template <class T>
void bounds_check(T t, int index, std::string_view name) {
    if (index < 0) {
        runtime_error(fmt::format("Cannot index {} with negative index [{}]",
                                  name, index));
    }

    int size = static_cast<int>(t.length());
    if (index >= size) {
        runtime_error(
            fmt::format("Index out-of-bounds error. Error indexing {} of "
                        "size {} with index {}",
                        name, size, index));
    }
}

Value bounds_checked_index(const String& target, int index) {
    bounds_check(target.value(), index, "string");
    char value_at_index = target.value()[index];
    return Value::create_string(std::string(1, value_at_index));
}

Value bounds_checked_index(const Vector& target, int index) {
    bounds_check(target, index, "vector");
    return Value::copy(target.at(index));
}

void Interpreter::index_move(const RelocatedQuad& quad) {
    int index = resolve_to_value(quad.src_b()).as_integer().value();

    Value target = resolve_to_value(quad.src_a());

    if (!target.is_object()) {
        ASSERT_NOT_REACHED();
        return;
    }

    Value result = [&]() {
        if (target.is_string()) {
            return bounds_checked_index(target.as_string(), index);
        } else if (target.is_vector()) {
            return bounds_checked_index(target.as_vector(), index);
        }
        ASSERT_NOT_REACHED();
    }();

    set_register(quad.dest().as_register(), std::move(result));
}

void Interpreter::index_assign(const RelocatedQuad& quad) {
    int index = resolve_to_value(quad.src_a()).as_integer().value();
    Vector& indexed = resolve_to_value(quad.dest()).as_vector();

    Value value = resolve_to_value(quad.src_b());

    bounds_check(indexed, index, "vector");
    indexed.set(index, value);
}

void Interpreter::interpret_and(const RelocatedQuad& quad) {
    compare(quad, [](Value lhs, Value rhs) {
        return lhs.is_truthy() && rhs.is_truthy();
    });
}

void Interpreter::interpret_or(const RelocatedQuad& quad) {
    compare(quad, [](const Value& lhs, const Value& rhs) {
        return lhs.is_truthy() || rhs.is_truthy();
    });
}

void Interpreter::alloc_regs(const RelocatedQuad& quad) {
  ASSERT(quad.opcode() == OPCode::ALLOC_REGS);
  Value count = resolve_to_value(quad.src_a());
  m_register_windows.emplace(count.as_integer().value());
}

unsigned Interpreter::take_pending_type_id() {
    ASSERT(m_pending_return_type.has_value());
    unsigned tmp = m_pending_return_type.value();
    m_pending_return_type.reset();
    return tmp;
}

void Interpreter::set_pending_type_id(unsigned value) {
    ASSERT(!m_pending_return_type.has_value());
    m_pending_return_type = value;
}

std::optional<Value> Interpreter::call_c_func(std::string_view lib,
                                              std::string_view func,
                                              const std::vector<Value>& args,
                                              unsigned return_type_id) {
    Result<dynlib::DynamicLibrary*> loaded_lib_or_error = dynlib::load_lib(lib);
    if (loaded_lib_or_error.is_error()) {
        ASSERT_NOT_REACHED_MSG("Could not load expected library");
    }

    dynlib::DynamicLibrary* loaded_lib = loaded_lib_or_error.get();
    ASSERT(loaded_lib->has_symbol(func));
    dynlib::DynamicLibrary::Callable callable = loaded_lib->get_callable(func);

    std::vector<ptr_t<vm::CTypeWrapper>> wrapped_args;

    for (Value op : args) {
        wrapped_args.push_back(vm::CTypeWrapper::from(op));
    }

    ctype::TypeInfo type_info = ctype::from_type_id(return_type_id);
    vm::CResultWrapper result_wrapper { type_info.c_type };

    switch (wrapped_args.size()) {
        case 0:
            callable.call();
            break;
        case 1:
            callable.call<1>(result_wrapper, wrapped_args);
            break;
        case 2:
            callable.call<2>(result_wrapper, wrapped_args);
            break;

        default:
            ASSERT_NOT_REACHED_MSG(fmt::format(
                "Unsupported number of arguments passed to C function: {}",
                wrapped_args.size()));
    }

    switch (type_info.seal_type.primitive()) {
        case Primitive::VOID: {
            return std::nullopt;
        }
        case Primitive::INT: {
            unsigned long val =
                *(static_cast<unsigned long*>(result_wrapper.buffer()));
            return Value::create_integer(val);
        }
        case Primitive::FLOAT: {
            double val = *(reinterpret_cast<double*>(result_wrapper.buffer()));
            return Value::create_real(val);
        }
        case Primitive::STRING: {
            std::string val =
                *(reinterpret_cast<char**>(result_wrapper.buffer()));
            return Value::create_string(val);
        }
        default:
            ASSERT_NOT_REACHED();
    }

    return std::nullopt;
}

void Interpreter::handle_crash() {
    fmt::print("The interpreter crashed unexpectedly. Dumping state:\n");
    fmt::print("Sequencer:\n");
    sequencer().dump();
}

void Interpreter::interpret_quad(const RelocatedQuad& quad) {
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
        case OPCode::CALL:
            call(quad);
            break;
        case OPCode::CALL_C:
            call_c(quad);
            break;
        case OPCode::SET_RET_TYPE:
            set_ret_type(quad);
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
        case OPCode::ALLOC_REGS:
            alloc_regs(quad);
            break;
        default:
            ASSERT_NOT_REACHED_MSG(fmt::format("Invalid OPCode: {}",
                                               opcode_to_string(quad.opcode()))
                                       .c_str());
    }
}
