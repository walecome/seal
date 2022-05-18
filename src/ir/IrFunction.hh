#pragma once

#include <map>
#include <optional>
#include <set>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Constants.hh"
#include "Operand.hh"
#include "Quad.hh"
#include "Register.hh"
#include "utility/StringTable.hh"

class FunctionDecl;

template <class ValueType, class T = decltype(ValueType().value)>
ValueOperand create_value_operand(T value) {
    return ValueOperand { ValueType { value } };
}

class IrFunction {
    MAKE_NONCOPYABLE(IrFunction)
    MAKE_NONMOVABLE(IrFunction)

   public:
    IrFunction(const FunctionDecl *decl) : m_decl { decl } {
    }

    void replace_prologue(Operand start, Operand end);

    // Construct a quad with the given opcode and operands, placing it in
    // the m_quads vector. Also binds any queued lables to the quad and
    // clears the label queue.
    // @FIXME: Should not take by value
    void add_quad(OPCode, Operand, Operand, Operand);

    static ValueOperand create_immediate_int(unsigned long value);
    static ValueOperand create_immediate_string(StringTable *string_table,
                                                std::string_view value);
    static ValueOperand create_immediate_real(double value);
    static ValueOperand create_vector_operand();

    LabelOperand create_label() const;

    template <class F>
    inline Register create_variable(const std::string_view name,
                                    F create_register,
                                    bool traverse_parent = true) {
        std::optional<Register> existing_reg =
            find_variable(name, traverse_parent);
        if (existing_reg) {
            return existing_reg.value();
        }
        auto reg = create_register();
        m_variables.back().insert({ name, reg });
        return reg;
    }

    std::optional<Register> find_variable(std::string_view name,
                                          bool recursive) const;
    static std::optional<Register> find_variable(
        const std::map<std::string_view, Register> &vars,
        const std::string_view name);

    FunctionOperand create_function_from_id(unsigned) const;

    LabelOperand create_and_queue_label();

    // Add a label to waiting labels
    void queue_label(const LabelOperand &);

    // Binds the queued labels to the given quad. Also clears the label
    // queue.
    void bind_queued_labels(size_t);

    void dump_quads() const;

    // Returns the name bound to variable_id if present, otherwise
    // tmp#<variable_id>.
    std::string resolve_variable_name(unsigned) const;

    auto declaration() const {
        return m_decl;
    }

    template <class F>
    void for_each_quad(F callback) {
        for (const ptr_t<Quad> &quad_ptr : m_quads) {
            callback(quad_ptr.get());
        }
    }

    unsigned id() const;

    // Return the index of the quad bound to label
    size_t quad_idx(const LabelOperand) const;

    LabelOperand get_epilogue_label() const {
        return m_epilogue_label;
    }

    void enter_block();
    void exit_block();

   private:
    // Bind the given label id to the given quad
    void bind_label(LabelOperand, size_t);

    const FunctionDecl *m_decl;
    const LabelOperand m_epilogue_label { create_label() };

    std::vector<ptr_t<Quad>> m_quads {};
    std::vector<std::map<std::string_view, Register>> m_variables {};
    std::map<LabelOperand, size_t> m_labels {};
    std::vector<LabelOperand> m_waiting_labels {};
};
