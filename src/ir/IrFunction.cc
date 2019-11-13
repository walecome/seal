#include <iostream>

#include "IrFunction.hh"
#include "ast/FunctionDecl.hh"

unsigned new_label_id() {
    static unsigned label_id = 0;
    return label_id++;
}

unsigned new_variable_id() {
    static unsigned label_id = 0;
    return label_id++;
}

Operand IrFunction::create_immediate(unsigned long value) {
    Operand operand {};
    operand.kind() = OperandKind::IMMEDIATE;
    operand.data().immediate_value = value;

    return operand;
}

Operand IrFunction::create_label() {
    Operand operand {};
    operand.kind() = OperandKind::LABEL;
    operand.data().variable_id = new_label_id();

    return operand;
}

Operand IrFunction::create_variable() {
    Operand operand {};
    operand.kind() = OperandKind::VARIABLE;
    operand.data().variable_id = new_variable_id();

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
    bind_queued_labels(quad.get());
    m_quads.push_back(std::move(quad));
}

void IrFunction::queue_label(const Operand &label) {
    ASSERT(label.is_label());

    m_waiting_labels.push_back(label.data().label_id);
}

void IrFunction::bind_queued_labels(const Quad *quad) {
    if (m_waiting_labels.empty()) return;

    for (auto label_id : m_waiting_labels) {
        bind_label(label_id, quad);
    }

    m_waiting_labels.clear();
}

void IrFunction::bind_label(const Operand &label, const Quad *quad) {
    ASSERT(label.is_label());

    bind_label(label.data().label_id, quad);
}

void IrFunction::bind_label(unsigned label_id, const Quad *quad) {
    ASSERT(m_labels.find(label_id) == std::end(m_labels));

    m_labels.insert({ label_id, quad });
}

void IrFunction::bind_variable(const Operand &variable,
                               const std::string_view var_name) {
    ASSERT(variable.is_variable());

    m_variable_ref.insert_or_assign(variable.data().variable_id, var_name);
}

void IrFunction::dump_quads() const {
    for (auto &quad : m_quads) {
        std::cout << quad->to_string() << std::endl;
    }
}