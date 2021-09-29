#include <fmt/format.h>
#include <memory>
#include <vector>
#include <utility>

#include "IrFunction.hh"
#include "Operand.hh"
#include "ast/FunctionDecl.hh"

unsigned new_label_id() {
    static unsigned label_id = 0;
    return label_id++;
}

ValueOperand IrFunction::create_immediate_int(unsigned long value) const {
    return create_value_operand<IntOperand>(value);
}

ValueOperand IrFunction::create_immediate_string(StringTable* string_table, std::string_view value) const {
    StringTable::Entry inserted = string_table->add(std::string(value));
    return create_value_operand<StringOperand>(inserted.key);
}

ValueOperand IrFunction::create_immediate_real(double value) const {
    return create_value_operand<RealOperand>(value);
}

ValueOperand IrFunction::create_vector_operand() const {
    return ValueOperand{ VectorOperand{} };
}

LabelOperand IrFunction::create_label() const {
    return LabelOperand { new_label_id() };
}

FunctionOperand IrFunction::create_function_from_id(unsigned function_id) const {
    return FunctionOperand{ function_id };
}

LabelOperand IrFunction::create_and_queue_label() {
    auto label = create_label();
    queue_label(label);
    return label;
}

void IrFunction::replace_prologue(QuadSource start, QuadSource end) {
    ASSERT(m_quads.at(0)->opcode() == OPCode::SAVE);
    ptr_t<Quad> quad = std::make_unique<Quad>(OPCode::SAVE, QuadDest {}, start, end);
    m_quads.at(0) = std::move(quad);
}

void IrFunction::add_quad(OPCode op_code, QuadDest dest, QuadSource src_a,
                          QuadSource src_b) {
    m_quads.push_back(std::make_unique<Quad>(op_code, dest, src_a, src_b));
    bind_queued_labels(m_quads.size() - 1);
}

void IrFunction::queue_label(const LabelOperand &label) {
    m_waiting_labels.push_back(label);
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
    
    m_quads.at(quad_idx)->add_label(label);

    m_labels.insert({ label, quad_idx });
}

void IrFunction::dump_quads() const {
    for (auto &quad : m_quads) {
        std::cout << quad->to_string() << std::endl;
    }
}

unsigned IrFunction::id() const { return declaration()->function_id(); }

size_t IrFunction::quad_idx(const LabelOperand label) const {
    return m_labels.at(label);
}
