#include "Operator.hh"

static std::string_view from_sym(OperatorSym op_sym) {
    for (auto x : string_to_op_sym) {
        if (x.second == op_sym) return x.first;
    }

    throw 1;
}

bool Operator::precedes(const Operator &other) const {
    return operator_precedence[operator_symbol] >
           operator_precedence[other.operator_symbol];
}

bool Operator::is_right_associative() const {
    return operator_symbol == OperatorSym::ASSIGN;
}

bool Operator::is_left_associative() const { return !is_right_associative(); }

std::string Operator::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << "{" << name() << ", "
        << from_sym(operator_symbol) << "}";
    return oss.str();
}