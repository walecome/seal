#include <iostream>

#include "IrFunction.hh"
#include "ast/FunctionDecl.hh"

unsigned new_label_id() {
    static unsigned label_id = 0;
    return label_id++;
}

unsigned new_variable_id() {
    static unsigned variable_id = 0;
    return variable_id++;
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

Operand IrFunction::create_variable_from_id(unsigned id) const {
    Operand operand { OperandKind::VARIABLE, VariableOperand { id } };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_variable() const {
    return create_variable_from_id(new_variable_id());
}

Operand IrFunction::get_variable(std::string_view identifier) const {
    auto it = m_varname_to_id.find(identifier);

    ASSERT(it != std::end(m_varname_to_id));

    return create_variable_from_id(it->second);
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

    quads().at(quad_idx)->set_label(label);

    m_labels.insert({ label, quad_idx });
}

void IrFunction::bind_variable(const Operand &variable,
                               const std::string_view var_name) {
    ASSERT(variable.is_variable());

    VariableOperand var_raw = std::get<VariableOperand>(variable.data());

    m_varname_to_id.insert_or_assign(var_name, var_raw);
    m_variable_ref.insert_or_assign(var_raw, var_name);
}

void IrFunction::dump_quads() const {
    for (auto &quad : m_quads) {
        std::cout << quad->to_string() << std::endl;
    }
}

std::string_view IrFunction::resolve_variable_name(unsigned variable_id) const {
    auto it = m_variable_ref.find(variable_id);

    if (it != std::end(m_variable_ref)) {
        return it->second;
    }

    std::ostringstream oss {};
    oss << "tmp#" << variable_id;
    return oss.str();
}

std::vector<const Quad *> IrFunction::quads_as_pointers() const {
    std::vector<const Quad *> quad_ptrs {};

    for (auto &unique_quad_ptr : m_quads) {
        quad_ptrs.push_back(unique_quad_ptr.get());
    }

    return quad_ptrs;
}

void IrFunction::__dump_variables() const {
    for (auto &x : m_variable_ref) {
        std::cout << x.first << "=" << x.second << std::endl;
    }
}

unsigned IrFunction::id() const { return declaration()->function_id(); }

size_t IrFunction::quad_idx(const LabelOperand label) const {
    return m_labels.at(label);
}