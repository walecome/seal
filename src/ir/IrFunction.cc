#include <fmt/format.h>

#include "IrFunction.hh"
#include "ast/FunctionDecl.hh"

unsigned new_label_id() {
    static unsigned label_id = 0;
    return label_id++;
}

template <class ValueType, class T = decltype(ValueType().value)>
ValueOperand create_value_operand(T value) {
    return ValueOperand { ValueType { value } };
}

Operand IrFunction::create_immediate_int(unsigned long value) const {
    Operand operand { OperandKind::IMMEDIATE_INT,
                      create_value_operand<IntOperand>(value) };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_immediate_string(std::string_view value) const {
    Operand operand { OperandKind::IMMEDIATE_STRING,
                      create_value_operand<StringOperand>(value) };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_immediate_real(double value) const {
    Operand operand { OperandKind::IMMEDIATE_REAL,
                      create_value_operand<RealOperand>(value) };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_label() const {
    Operand operand { OperandKind::LABEL, LabelOperand { new_label_id() } };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_variable(const std::string_view identifier) const {
    Operand operand { OperandKind::VARIABLE, VariableOperand { identifier } };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_tmp_variable() {
    static unsigned variable_count = 0;

    // This is kind of ugly... VariableOperand keeps a std::string_view to the
    // variable identifier. When creating a temporary variable we need the name
    // (tmp$NUM) to have a lifetime as least as long as the VariableOperand. We
    // insert the std::string name into a set, and the reference it by a
    // std::string_view object. The first implementation used a std::vector,
    // which would invalidate the std::string_view's when resized. We should
    // probably have a better solution for this...

    // m_tmp_variables own the string object for the tmp variable identifier.
    auto inserted =
        m_tmp_variables.emplace(fmt::format("temp${}", variable_count++));
    Operand operand { OperandKind::VARIABLE,
                      VariableOperand { *inserted.first } };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_function_from_id(unsigned function_id) const {
    Operand operand { OperandKind::FUNCTION, FunctionOperand { function_id } };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_and_queue_label() {
    auto label = create_label();
    queue_label(label);
    return label;
}

void IrFunction::add_quad(OPCode op_code, Operand dest, Operand src_a,
                          Operand src_b) {
    auto quad = std::make_unique<Quad>(op_code, dest, src_a, src_b);
    m_quads.push_back(std::move(quad));
    bind_queued_labels(m_quads.size() - 1);
}

void IrFunction::queue_label(const Operand &label) {
    ASSERT(label.is_label());
    m_waiting_labels.push_back(std::get<LabelOperand>(label.data()));
}

void IrFunction::bind_queued_labels(size_t quad_idx) {
    if (m_waiting_labels.empty()) return;

    for (auto label_id : m_waiting_labels) {
        bind_label(label_id, quad_idx);
    }

    m_waiting_labels.clear();
}

void IrFunction::bind_label(LabelOperand label, size_t quad_idx) {
    ASSERT(m_labels.find(label) == std::end(m_labels));

    quads().at(quad_idx)->add_label(label);

    m_labels.insert({ label, quad_idx });
}

void IrFunction::dump_quads() const {
    for (auto &quad : m_quads) {
        std::cout << quad->to_string() << std::endl;
    }
}

std::vector<const Quad *> IrFunction::quads_as_pointers() const {
    std::vector<const Quad *> quad_ptrs {};

    for (auto &unique_quad_ptr : m_quads) {
        quad_ptrs.push_back(unique_quad_ptr.get());
    }

    return quad_ptrs;
}

unsigned IrFunction::id() const { return declaration()->function_id(); }

size_t IrFunction::quad_idx(const LabelOperand label) const {
    return m_labels.at(label);
}