#include <fmt/format.h>
#include <array>
#include <string_view>
#include <vector>

#include "CTypeWrapper.hh"
#include "Constants.hh"
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

Value create_from_value_operand(const ValueOperand& value_operand) {
  // TODO: Implement
  ASSERT_NOT_REACHED_MSG("create_from_value_operand()");
}

Value create_from_string(String value) {
  // TODO: Implement
  ASSERT_NOT_REACHED_MSG("create_from_string()");
}

Value create_from_boolean(Boolean value) {
  // TODO: Implement
  ASSERT_NOT_REACHED_MSG("create_from_boolean()");
}

Value create_from_integer(Integer value) {
  // TODO: Implement
  ASSERT_NOT_REACHED_MSG("create_from_integer()");
}

Value create_from_real(Real value) {
  // TODO: Implement
  ASSERT_NOT_REACHED_MSG("create_from_real()");
}

Register return_register() {
  return Register(0);
}

}  // namespace

Interpreter::Interpreter(const QuadCollection& quads, StringTable* string_table,
                         bool verbose)
    : m_quads { quads },
      m_string_table { string_table },
      m_registers(std::vector<Value>(quads.register_count)),
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

Value Interpreter::resolve_register(Register reg) const {
    return m_registers[reg.index()];
}

void Interpreter::set_register(Register reg, Value value) {
    m_registers[reg.index()] = std::move(value);
}

unsigned Interpreter::resolve_label(const Operand& dest) const {
    return dest.as_label().value;
}

Value Interpreter::resolve_to_value(const Operand& source) const {
    // TODO: Support functions
    ASSERT(!source.is_function());
    if (source.is_value()) {
        return create_from_value_operand(source.as_value());
    }

    if (source.is_register()) {
        return resolve_register(source.as_register());
    }

    ASSERT_NOT_REACHED_MSG(
        fmt::format("Invalid QuadSource type: {}", source.to_debug_string())
            .c_str());
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

struct BinOpPlusVisitor {
    BinOpPlusVisitor(StringTable* string_table) : string_table(string_table) {}

    StringTable* string_table;

    template <typename T, typename U>
    Value operator()(T, U) {
        ASSERT_NOT_REACHED();
    }

    Value operator()(String a, String b) {
        std::string result = a.resolve(string_table) + b.resolve(string_table);
        // TODO: Resulting String should not be inserted in string table.
        StringTable::Entry entry = string_table->add(std::move(result));
        return create_from_string(String {entry});
    }

    Value operator()(Vector, Vector) { ASSERT_NOT_REACHED(); }

    template <typename T>
    Value operator()(T a, T b) {
        return Value { T { a + b } };
    }
};

template <class Operator>
void binop_helper(Interpreter* interpreter, const Quad& quad) {
    Value lhs = interpreter->resolve_to_value(quad.src_a());
    Value rhs = interpreter->resolve_to_value(quad.src_b());

    Value result =
        std::visit(BinOpVisitor<Operator> {}, lhs.data(), rhs.data());

    interpreter->set_register(quad.dest().as_register(), result);
}

void plus_helper(Interpreter* interpreter, const Quad& quad,
                 StringTable* string_table) {
    Value lhs = interpreter->resolve_to_value(quad.src_a());
    Value rhs = interpreter->resolve_to_value(quad.src_b());

    Value result = std::visit(BinOpPlusVisitor { string_table }, lhs.data(), rhs.data());

    interpreter->set_register(quad.dest().as_register(), result);
}

void Interpreter::add(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::ADD);
    plus_helper(this, quad, m_string_table);
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
    CmpVisitor(StringTable* string_table) : string_table(string_table) {}

    StringTable* string_table;

    template <typename T, typename U>
    Value operator()(T, U) {
        ASSERT_NOT_REACHED();
    }

    template <typename T>
    Value operator()(T a, T b) {
        return Operator {}(a.value, b.value);
    }

    template <>
    Value operator()(StringOperand a, StringOperand b) {
        return Operator {}(*a.resolve(), *b.resolve());
    }
};

template <class Operator>
void cmp_helper(Interpreter* interpreter, StringTable* string_table,
                const Quad& quad) {
    Value lhs = interpreter->resolve_to_value(quad.src_a());
    Value rhs = interpreter->resolve_to_value(quad.src_b());

    Value result = std::visit(CmpVisitor<Operator> { string_table }, lhs.data(), rhs.data());
    interpreter->set_register(quad.dest().as_register(), result);
}

void Interpreter::cmp_eq(const Quad& quad) {
    cmp_helper<std::equal_to<>>(this, m_string_table, quad);
}

void Interpreter::cmp_gt(const Quad& quad) {
    cmp_helper<std::greater<>>(this, m_string_table, quad);
}

void Interpreter::cmp_lt(const Quad& quad) {
    cmp_helper<std::less<>>(this, m_string_table, quad);
}

void Interpreter::cmp_gteq(const Quad& quad) {
    cmp_helper<std::greater_equal<>>(this, m_string_table, quad);
}

void Interpreter::cmp_lteq(const Quad& quad) {
    cmp_helper<std::less_equal<>>(this, m_string_table, quad);
}

void Interpreter::cmp_noteq(const Quad& quad) {
    cmp_helper<std::not_equal_to<>>(this, m_string_table, quad);
}
void Interpreter::jmp(const Quad& quad) {
    unsigned label_id = resolve_label(quad.dest());

    auto target_quad = m_quads.label_to_quad.find(label_id);
    ASSERT(target_quad != m_quads.label_to_quad.end());
    current_frame()->set_program_counter(target_quad->second);
    current_frame()->set_jump_performed(true);
}

void Interpreter::jmp_z(const Quad& quad) {
    Value condition = resolve_to_value(quad.src_a());
    if (condition.as_boolean().value()) {
        jmp(quad);
    }
}

void Interpreter::jmp_nz(const Quad& quad) {
    Value condition = resolve_to_value(quad.src_a());
    if (!condition.as_boolean().value()) {
        jmp(quad);
    }
}

void Interpreter::push_arg(const Quad& quad) {
    Value value = resolve_to_value(quad.src_a());
    m_arguments.push(value);
}

void Interpreter::pop_arg(const Quad& quad) {
    ASSERT(!m_arguments.empty());

    Value argument = m_arguments.front();
    m_arguments.pop();

    set_register(quad.dest().as_register(), argument );
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
        std::vector<Value> args {};
        while (!m_arguments.empty()) {
            args.push_back(m_arguments.front());
            m_arguments.pop();
        }
        Value ret = BuiltIn::call_builtin_function(func, args);
        set_register(quad.dest().as_register(), ret);
        return;
    }

    enter_new_frame();

    // Program counter will be incremented in interpret function
    current_frame()->set_program_counter(m_quads.function_to_quad.at(func) - 1);
}

void Interpreter::call_c(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::CALL_C);

    std::vector<Value> args {};
    while (!m_arguments.empty()) {
        args.push_back(m_arguments.front());
        m_arguments.pop();
    }

    StringTable::Key lib = quad.src_a().as_value().as_string();
    StringTable::Key func = quad.src_b().as_value().as_string();
    std::optional<Value> return_value =
        call_c_func(lib, func, args, take_pending_type_id());

    if (return_value.has_value()) {
        set_register(quad.dest().as_register(), *return_value);
    }
}

void Interpreter::set_ret_type(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::SET_RET_TYPE);
    set_pending_type_id(quad.src_a().as_value().as_int());
}

void Interpreter::ret(const Quad& quad) {
    auto source = quad.src_a();

    if (current_frame()->is_main_frame()) {
        Value value = resolve_register(return_register());
        exit(value.as_integer().value());
    }

    exit_frame();
}

void Interpreter::move(const Quad& quad) {
    ASSERT(quad.opcode() == OPCode::MOVE);
    Value source = resolve_to_value(quad.src_a());
    if (source.is_vector()) {
        // TODO: Need to figure out if we want to copy here or not. It bascially comes down to
        //       which type of value semantics we for objects like array (possible string).
        //       Currently leaning towards a Python style, which would mean we need to add
        //       copy functionality to the language.
        ASSERT_NOT_REACHED_MSG("TODO: Interpreter::move array");
        // source = source.as_vector();
    }
    set_register(quad.dest().as_register(), source);
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

void bounds_check(Vector vec, int index) {
    if (index < 0) {
        runtime_error(
            fmt::format("Cannot index vector with negative index [{}]", index));
    }

    auto& values = vec.values();

    int size = static_cast<int>(values.size());
    if (index >= size) {
        runtime_error(
            fmt::format("Index out-of-bounds error. Error indexing vector of "
                        "size {} with index {}",
                        size, index));
    }
}

String bounds_checked_index(String target, int index, StringTable* string_table) {
  std::string_view resolved = target.resolve(string_table);
  bounds_check(resolved, index);

  char value_at_index = resolved[index];
  // FIXME: We really shouldn't add runtime strings to the string table.
  StringTable::Entry entry = string_table->add(value_at_index);
  return String(entry);
}

Value bounds_checked_index(Vector target, int index) {
    bounds_check(target, index);
    return target.values().at(index);
}

void Interpreter::index_move(const Quad& quad) {
    int index = resolve_to_value(quad.src_b()).as_integer().value();

    Value target = resolve_to_value(quad.src_a());

    Value value;

    if (target.is_string()) {
      value = create_from_string(bounds_checked_index(target.as_string(), index, m_string_table));
    } else if (target.is_vector()) {
      value = bounds_checked_index(target.as_vector(), index);
    } else {
        ASSERT_NOT_REACHED();
    }

    set_register(quad.dest().as_register(), value);
}

void Interpreter::index_assign(const Quad& quad) {
    int index = resolve_to_value(quad.src_a()).as_integer().value();
    Vector indexed = resolve_to_value(quad.dest()).as_vector();

    Value value = resolve_to_value(quad.src_b());

    bounds_check(indexed, index);
    indexed.mutable_values().at(index) = value;
}

void Interpreter::interpret_and(const Quad& quad) {
    Value lhs = resolve_to_value(quad.src_a());
    Value rhs = resolve_to_value(quad.src_b());

    Boolean result = Boolean(lhs.as_boolean().value() && rhs.as_boolean().value());
    set_register(quad.dest().as_register(), create_from_boolean(result));
}

void Interpreter::interpret_or(const Quad& quad) {
    Value lhs = resolve_to_value(quad.src_a());
    Value rhs = resolve_to_value(quad.src_b());

    Boolean result = Boolean(lhs.as_boolean().value() || rhs.as_boolean().value());
    set_register(quad.dest().as_register(), create_from_boolean(result));
}

StackFrame* Interpreter::current_frame() { return &m_stack_frames.top(); }

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

std::optional<Value> Interpreter::call_c_func(
    StringTable::Key lib, StringTable::Key func,
    const std::vector<Value>& args, unsigned return_type_id) {
    Result<dynlib::DynamicLibrary*> loaded_lib_or_error =
        dynlib::load_lib(*m_string_table->get_at(lib));
    if (loaded_lib_or_error.is_error()) {
        ASSERT_NOT_REACHED_MSG("Could not load expected library");
    }

    dynlib::DynamicLibrary* loaded_lib = loaded_lib_or_error.get();
    ASSERT(loaded_lib->has_symbol(*m_string_table->get_at(func)));
    dynlib::DynamicLibrary::Callable callable =
        loaded_lib->get_callable(*m_string_table->get_at(func));

    std::vector<ptr_t<vm::CTypeWrapper>> wrapped_args;

    for (const Value& op : args) {
        wrapped_args.push_back(vm::CTypeWrapper::from(m_string_table, op));
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
            return create_from_integer(Integer(val));
        }
        case Primitive::FLOAT: {
            double val = *(reinterpret_cast<double*>(result_wrapper.buffer()));
            return create_from_real(Real(val));
        }
        case Primitive::STRING: {
            std::string val =
                *(reinterpret_cast<char**>(result_wrapper.buffer()));
            // FIXME: Should avoid adding runtime strings to the string table.
            StringTable::Entry entry = m_string_table->add(std::move(val));
            return create_from_string(String(entry));
        }
        default:
            ASSERT_NOT_REACHED();
    }

    return {};
}
