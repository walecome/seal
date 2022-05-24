#include <fmt/format.h>
#include <array>
#include <string_view>
#include <vector>

#include "CTypeWrapper.hh"
#include "Constants.hh"
#include "CrashHelper.hh"
#include "Interpreter.hh"
#include "OPCode.hh"
#include "builtin/BuiltIn.hh"
#include "dynlib/DynLib.hh"
#include "dynlib/DynLibLoader.hh"
#include "fmt/core.h"
#include "ir/QuadCollection.hh"
#include "types/CType.hh"

namespace {

void runtime_error(const std::string& message) {
    fmt::print("{}\n", message);
    exit(EXIT_FAILURE);
}

Register return_register() {
    return Register(0);
}

}  // namespace

Interpreter::Interpreter(const QuadCollection& quads,
                         const ValuePool* constant_pool, bool verbose)
    : m_quads { quads },
      m_registers(std::vector<PoolEntry>(quads.register_count)),
      m_context(Context(constant_pool)),
      m_verbose { verbose } {
}

void Interpreter::interpret() {
    CrashHelper::ScopedHandler handler([this] { handle_crash(); });

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

Value& Interpreter::resolve_register(Register reg) const {
    return context().get_value(m_registers[reg.index()]);
}

void Interpreter::set_register(Register reg, PoolEntry entry) {
    m_registers[reg.index()] = entry;
}

unsigned Interpreter::resolve_label(const Operand& dest) const {
    return dest.as_label().value;
}

Value& Interpreter::resolve_to_value(const Operand& source) const {
    // TODO: Support functions
    ASSERT(!source.is_function());
    if (source.is_value_entry()) {
        return context().get_value(source.as_value_entry());
    }

    if (source.is_register()) {
        return resolve_register(source.as_register());
    }

    ASSERT_NOT_REACHED_MSG(
        fmt::format("Invalid QuadSource type: {}", source.to_debug_string())
            .c_str());
}

PoolEntry Interpreter::resolve_to_entry(const Operand& source) const {
    if (source.is_value_entry()) {
        return source.as_value_entry();
    }

    if (source.is_register()) {
        return m_registers.at(source.as_register().index());
    }

    ASSERT_NOT_REACHED_MSG(
        fmt::format("Invalid QuadSource type: {}", source.to_debug_string())
            .c_str());
}

namespace {
template <class BinaryOperator>
PoolEntry compute_binary_expression(Context& context, PoolEntry lhs,
                                    PoolEntry rhs) {
    const Value& first = context.get_value(lhs);
    const Value& second = context.get_value(rhs);

    ASSERT(first.type() == second.type());

    auto apply_binary_operator = [&](const auto& a, const auto& b) {
        return BinaryOperator {}(a, b);
    };

    switch (first.type()) {
        case ValueType::Boolean:
            return context.dynamic_pool().create_boolean(apply_binary_operator(
                first.as_boolean().value(), second.as_boolean().value()));
        case ValueType::Integer:
            return context.dynamic_pool().create_integer(apply_binary_operator(
                first.as_integer().value(), second.as_integer().value()));
        case ValueType::Real:
            return context.dynamic_pool().create_real(apply_binary_operator(
                first.as_real().value(), second.as_real().value()));
        case ValueType::String:
            ASSERT_NOT_REACHED();
        case ValueType::Vector:
            ASSERT_NOT_REACHED();
    }
}

PoolEntry concatenate_strings(Context& context, const String& lhs,
                              const String& rhs) {
    std::string result = std::string(lhs.value()) + std::string(rhs.value());
    return context.dynamic_pool().create_string(result);
}

template <>
PoolEntry compute_binary_expression<std::plus<>>(Context& context,
                                                 PoolEntry lhs, PoolEntry rhs) {
    const Value& first = context.get_value(lhs);
    const Value& second = context.get_value(rhs);

    ASSERT(first.type() == second.type());

    auto apply_binary_operator = [&](const auto& a, const auto& b) {
        return std::plus<> {}(a, b);
    };

    switch (first.type()) {
        case ValueType::Boolean:
            return context.dynamic_pool().create_boolean(apply_binary_operator(
                first.as_boolean().value(), second.as_boolean().value()));
        case ValueType::Integer:
            return context.dynamic_pool().create_integer(apply_binary_operator(
                first.as_integer().value(), second.as_integer().value()));
        case ValueType::Real:
            return context.dynamic_pool().create_real(apply_binary_operator(
                first.as_real().value(), second.as_real().value()));
        case ValueType::String:
            return concatenate_strings(context, first.as_string(),
                                       second.as_string());
        case ValueType::Vector:
            ASSERT_NOT_REACHED();
    }
}

}  // namespace

void Interpreter::add(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::ADD);

    PoolEntry lhs = resolve_to_entry(quad.src_a());
    PoolEntry rhs = resolve_to_entry(quad.src_b());
    PoolEntry result =
        compute_binary_expression<std::plus<>>(context(), lhs, rhs);

    set_register(quad.dest().as_register(), result);
}

void Interpreter::sub(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::SUB);

    PoolEntry lhs = resolve_to_entry(quad.src_a());
    PoolEntry rhs = resolve_to_entry(quad.src_b());
    PoolEntry result =
        compute_binary_expression<std::minus<>>(context(), lhs, rhs);

    set_register(quad.dest().as_register(), result);
}
void Interpreter::mult(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::MULT);

    PoolEntry lhs = resolve_to_entry(quad.src_a());
    PoolEntry rhs = resolve_to_entry(quad.src_b());
    PoolEntry result =
        compute_binary_expression<std::multiplies<>>(context(), lhs, rhs);

    set_register(quad.dest().as_register(), result);
}

void Interpreter::div(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::DIV);

    PoolEntry lhs = resolve_to_entry(quad.src_a());
    PoolEntry rhs = resolve_to_entry(quad.src_b());
    PoolEntry result =
        compute_binary_expression<std::divides<>>(context(), lhs, rhs);

    set_register(quad.dest().as_register(), result);
}

void Interpreter::compare(
    const Quad& quad,
    std::function<bool(const Value&, const Value&)> comparison_predicate) {
    const Value& lhs = resolve_to_value(quad.src_a());
    const Value& rhs = resolve_to_value(quad.src_b());

    bool result = comparison_predicate(lhs, rhs);
    PoolEntry result_entry = context().dynamic_pool().create_boolean(result);
    set_register(quad.dest().as_register(), result_entry);
}

void Interpreter::cmp_eq(const Quad& quad) {
    compare(quad,
            [](const Value& lhs, const Value& rhs) { return lhs == rhs; });
}

void Interpreter::cmp_gt(const Quad& quad) {
    compare(quad, [](const Value& lhs, const Value& rhs) { return lhs > rhs; });
}

void Interpreter::cmp_lt(const Quad& quad) {
    compare(quad, [](const Value& lhs, const Value& rhs) { return lhs < rhs; });
}

void Interpreter::cmp_gteq(const Quad& quad) {
    compare(quad,
            [](const Value& lhs, const Value& rhs) { return lhs >= rhs; });
}

void Interpreter::cmp_lteq(const Quad& quad) {
    compare(quad,
            [](const Value& lhs, const Value& rhs) { return lhs <= rhs; });
}

void Interpreter::cmp_noteq(const Quad& quad) {
    compare(quad,
            [](const Value& lhs, const Value& rhs) { return lhs != rhs; });
}

void Interpreter::jmp(const Quad& quad) {
    unsigned label_id = resolve_label(quad.dest());

    auto target_quad = m_quads.label_to_quad.find(label_id);
    ASSERT(target_quad != m_quads.label_to_quad.end());
    current_frame()->set_program_counter(target_quad->second);
    current_frame()->set_jump_performed(true);
}

void Interpreter::jmp_z(const Quad& quad) {
    const Value& condition = resolve_to_value(quad.src_a());
    if (condition.as_boolean().value()) {
        jmp(quad);
    }
}

void Interpreter::jmp_nz(const Quad& quad) {
    const Value& condition = resolve_to_value(quad.src_a());
    if (!condition.as_boolean().value()) {
        jmp(quad);
    }
}

void Interpreter::push_arg(const Quad& quad) {
    m_arguments.push(resolve_to_entry(quad.src_a()));
}

void Interpreter::pop_arg(const Quad& quad) {
    ASSERT(!m_arguments.empty());

    PoolEntry argument = m_arguments.front();
    m_arguments.pop();

    set_register(quad.dest().as_register(), std::move(argument));
}

void Interpreter::save(const Quad& quad) {
    int start_idx = quad.src_a().as_register().index();
    int end_index = quad.src_b().as_register().index();

    for (int i = start_idx; i <= end_index; ++i) {
        // TODO: Should we copy here?
        m_stack.push(m_registers.at(i));
    }
}

void Interpreter::restore(const Quad& quad) {
    int start_idx = quad.src_a().as_register().index();
    int end_index = quad.src_b().as_register().index();
    for (int i = end_index; i >= start_idx; --i) {
        set_register(Register(i), std::move(m_stack.top()));
        m_stack.pop();
    }
}

void Interpreter::call(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::CALL);

    FunctionOperand func = quad.src_a().as_function();

    if (BuiltIn::is_builtin(func)) {
        std::vector<PoolEntry> args {};
        while (!m_arguments.empty()) {
            args.push_back(m_arguments.front());
            m_arguments.pop();
        }
        PoolEntry ret = BuiltIn::call_builtin_function(func, args, context());
        set_register(quad.dest().as_register(), ret);
        return;
    }

    enter_new_frame();

    // Program counter will be incremented in interpret function
    current_frame()->set_program_counter(m_quads.function_to_quad.at(func) - 1);
}

void Interpreter::call_c(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::CALL_C);

    std::vector<PoolEntry> args {};
    while (!m_arguments.empty()) {
        args.push_back(std::move(m_arguments.front()));
        m_arguments.pop();
    }

    std::optional<PoolEntry> return_value = call_c_func(
        resolve_to_entry(quad.src_a()), resolve_to_entry(quad.src_b()), args,
        take_pending_type_id());

    if (return_value.has_value()) {
        set_register(quad.dest().as_register(), std::move(*return_value));
    }
}

void Interpreter::set_ret_type(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::SET_RET_TYPE);
    set_pending_type_id(resolve_to_value(quad.src_a()).as_integer().value());
}

void Interpreter::ret(const Quad&) {
    if (current_frame()->is_main_frame()) {
        const Value& value = resolve_register(return_register());
        exit(value.as_integer().value());
    }

    exit_frame();
}

void Interpreter::move(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::MOVE);
    PoolEntry source = resolve_to_entry(quad.src_a());
    if (resolve_to_value(quad.src_a()).is_vector()) {
        // TODO: Need to figure out if we want to copy here or not. It bascially
        // comes down to
        //       which type of value semantics we for objects like array
        //       (possible string). Currently leaning towards a Python style,
        //       which would mean we need to add copy functionality to the
        //       language.
        ASSERT_NOT_REACHED_MSG("TODO: Interpreter::move array");
        // source = source.as_vector();
    }
    // FIXME: Fix copy logic
    ASSERT_NOT_REACHED_MSG(
        "FIXME: source can be an entry in the constant pool, but need to copy "
        "to dynamic pool");
    set_register(quad.dest().as_register(),
                 context().dynamic_pool().copy(source));
}

void bounds_check(std::string_view s, int index) {
    if (index < 0) {
        runtime_error(
            fmt::format("Cannot index string with negative index [{}]", index));
    }

    int size = static_cast<int>(s.size());
    if (index >= size) {
        runtime_error(
            fmt::format("Index out-of-bounds error. Error indexing string of "
                        "size {} with index {}",
                        size, index));
    }
}

void bounds_check(const Vector& vec, int index) {
    if (index < 0) {
        runtime_error(
            fmt::format("Cannot index vector with negative index [{}]", index));
    }

    int size = static_cast<int>(vec.length());
    if (index >= size) {
        runtime_error(
            fmt::format("Index out-of-bounds error. Error indexing vector of "
                        "size {} with index {}",
                        size, index));
    }
}

PoolEntry bounds_checked_index(Context& context, const String& target,
                               int index) {
    bounds_check(target.value(), index);
    char value_at_index = target.value()[index];
    return context.dynamic_pool().create_string(std::string(1, value_at_index));
}

PoolEntry bounds_checked_index(const Vector& target, int index) {
    bounds_check(target, index);
    return target.at(index);
}

void Interpreter::index_move(const Quad& quad) {
    int index = resolve_to_value(quad.src_b()).as_integer().value();

    const Value& target = resolve_to_value(quad.src_a());

    PoolEntry result;

    if (target.is_string()) {
        result = bounds_checked_index(context(), target.as_string(), index);
    } else if (target.is_vector()) {
        result = bounds_checked_index(target.as_vector(), index);
    } else {
        ASSERT_NOT_REACHED();
    }

    set_register(quad.dest().as_register(), std::move(result));
}

void Interpreter::index_assign(const Quad& quad) {
    int index = resolve_to_value(quad.src_a()).as_integer().value();
    Vector& indexed = resolve_to_value(quad.dest()).as_vector();

    PoolEntry entry = resolve_to_entry(quad.src_b());

    bounds_check(indexed, index);
    indexed.set(index, entry);
}

void Interpreter::interpret_and(const Quad& quad) {
    compare(quad, [](const Value& lhs, const Value& rhs) {
        return bool(lhs) && bool(rhs);
    });
}

void Interpreter::interpret_or(const Quad& quad) {
    compare(quad, [](const Value& lhs, const Value& rhs) {
        return bool(lhs) || bool(rhs);
    });
}

StackFrame* Interpreter::current_frame() {
    return &m_stack_frames.top();
}

void Interpreter::enter_new_frame() {
    m_stack_frames.emplace(current_frame()->program_counter(), current_frame());
};

void Interpreter::exit_frame() {
    // fmt::print("exit_frame()\n");
    m_stack_frames.pop();
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

Context& Interpreter::context() {
    return m_context;
}

const Context& Interpreter::context() const {
    return m_context;
}

std::optional<PoolEntry> Interpreter::call_c_func(
    PoolEntry lib, PoolEntry func, const std::vector<PoolEntry>& args,
    unsigned return_type_id) {
    const Value& lib_name = context().get_value(lib);
    ASSERT(lib_name.is_string());
    const Value& func_name = context().get_value(func);
    ASSERT(func_name.is_string());

    Result<dynlib::DynamicLibrary*> loaded_lib_or_error =
        dynlib::load_lib(lib_name.as_string().value());
    if (loaded_lib_or_error.is_error()) {
        ASSERT_NOT_REACHED_MSG("Could not load expected library");
    }

    dynlib::DynamicLibrary* loaded_lib = loaded_lib_or_error.get();
    ASSERT(loaded_lib->has_symbol(func_name.as_string().value()));
    dynlib::DynamicLibrary::Callable callable =
        loaded_lib->get_callable(func_name.as_string().value());

    std::vector<ptr_t<vm::CTypeWrapper>> wrapped_args;

    for (const PoolEntry& op : args) {
        wrapped_args.push_back(vm::CTypeWrapper::from(context().get_value(op)));
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
            return {};
        }
        case Primitive::INT: {
            unsigned long val =
                *(static_cast<unsigned long*>(result_wrapper.buffer()));
            return context().dynamic_pool().create_integer(val);
        }
        case Primitive::FLOAT: {
            double val = *(reinterpret_cast<double*>(result_wrapper.buffer()));
            return context().dynamic_pool().create_real(val);
        }
        case Primitive::STRING: {
            std::string val =
                *(reinterpret_cast<char**>(result_wrapper.buffer()));
            return context().dynamic_pool().create_string(val);
        }
        default:
            ASSERT_NOT_REACHED();
    }

    return {};
}

void Interpreter::handle_crash() {
    fmt::print("The interpreter crashed unexpectedly. Dumping state:\n");
    fmt::print("Program counter: {}\n", current_frame()->program_counter());
    const Quad& quad = m_quads.quads[current_frame()->program_counter()];
    fmt::print("Quad: {}\n", quad.to_string());
    fmt::print("Dumping dynamic pool:\n");
    context().dynamic_pool().dump();
    fmt::print("Dumping registers:\n");
    for (size_t i = 0; i < m_registers.size(); ++i) {
        PoolEntry entry = m_registers.at(i);
        if (entry.has_value()) {
            fmt::print("r{}: {} ({})\n", i, entry.key(),
                       entry.type() == PoolEntry::Type::Constant ? "Constant"
                                                                 : "Dynamic");
        } else {
            fmt::print("r{}: empty\n", i);
        }
    }
}
